/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include <elfio/elfio.hpp>
#include "memory/memory.hpp"

namespace micromachine::system {
class loader {
private:
	// the byte used in lieu of zeroes when allocating segments
	static constexpr uint8_t DEBUG_FILL_BYTE = 0xCC;

	class segment_allocator;

public:
	class program {
		friend class segment_allocator;
	public:
		using ptr = std::shared_ptr<program>;
		using memory_segment = std::vector<uint8_t>;

	private:
		std::vector<memory_segment> _memory_segments;
		uint32_t _entry_point = 0;
		std::string _path;

	public:
		bool is_null() const {
			return _memory_segments.empty();
		}

		uint32_t entry_point() const {
			return _entry_point;
		}
		void set_entry_point(uint32_t address) {
			_entry_point = address;
		}

		const std::string& path() {
			return _path;
		}

		void set_path(const std::string& path) {
			_path = path;
		}

	};

private:
	class segment_allocator {
	private:
		// memory will be filled with debug bytes
		// if debug is enabled.
		const bool _non_zero_fill;

	public:
		segment_allocator(bool non_zero_fill)
			: _non_zero_fill(non_zero_fill) {}

		program::memory_segment& allocate_segment(program::ptr program, size_t size) {
			return program->_memory_segments.emplace_back(size, _non_zero_fill ? DEBUG_FILL_BYTE : 0);
		}
	};

public:
	static program::ptr load_elf(const std::string& path, memory& mem, bool non_zero_fill) {

		std::ifstream ifs(path, std::ios::binary);
		if(!ifs) {
			fprintf(stderr, "Failed to open file path=%s\n", path.c_str());
			return nullptr;
		}

		segment_allocator allocator(non_zero_fill);
		program::ptr prog = std::make_shared<program>();
		prog->set_path(path);

		ELFIO::elfio elfReader;
		if(!elfReader.load(ifs)) {
			return prog;
		}

		if(elfReader.get_class() != ELFCLASS32) {
			return prog;
		}

		if(elfReader.get_encoding() != ELFDATA2LSB) {
			return prog;
		}

		uint32_t entryPoint = (uint32_t)elfReader.get_entry();
		fprintf(stderr, "PC will be set to %08X\n", entryPoint);

		prog->set_entry_point(entryPoint);

		/* Process each segment defined in the ELF file
		 * and map them in the virtual memory in order for
		 * the executable to run properly.
		 *
		 * The data of segments containing pre-initialized data
		 * (referred as progbits in this context) is
		 * loaded from the ELF file.
		 *
		 * The data of segments containing non-initialized data
		 * are allocate don the fly.
		 *
		 * Relocations
		 * A segment is considered relocatable if it has pre-initialized data
		 * (aka: progbits) AND if its virtual address is different that its physical
		 * address. It this case, the progbits of the segment are mapped at the
		 * _physical_ address and an additional new segment of the appropriate
		 * _memory size_ is created and mapped (without any data) at the _virtual_ address.
		 * to allow proper relocation at runtime.
		 *
		 * Segments with different physical and virtual addresses who don't have
		 * progbits are simply mapped at the virtual address.
		 *
		 * The runtime is responsible for relocating a segment at startup by copying the
		 * progbits from the physical address to the memory address.
		 */
		for(const ELFIO::segment* elf_segment : elfReader.segments) {

			const uint32_t virtual_address = (uint32_t)elf_segment->get_virtual_address();
			const uint32_t physical_address = (uint32_t)elf_segment->get_physical_address();
			const uint32_t memory_size = (uint32_t)elf_segment->get_memory_size();
			const uint32_t progbits_size = (uint32_t)elf_segment->get_file_size();
			const uint32_t segment_index = elf_segment->get_index();
			const bool has_progbits = progbits_size != 0;
			const bool requires_relocation = has_progbits && virtual_address != physical_address;
			const std::string segment_name = std::string("seg." + std::to_string(segment_index));

			fprintf(stderr,
					"segment [%02i] : virtual=%08x physical=%08x map=%s size=%08x progbits=%s "
					"relocation=%s\n",
					elf_segment->get_index(),
					virtual_address,
					physical_address,
					requires_relocation ? "phys" : "virt",
					memory_size,
					has_progbits ? "yes" : "no",
					requires_relocation ? "yes" : "no ");

			program::memory_segment& segment = allocator.allocate_segment(prog, memory_size);

			// load the progbits in this segment, if any.
			if(has_progbits) {
				memcpy(segment.data(), elf_segment->get_data(), elf_segment->get_file_size());
			}

			if(requires_relocation) {
				// Relocation segment -> Map in physical address space
				//
				// Note that in order to avoid mapping unnecessary data
				// only the progbits part of the relocatable segment is mapped to
				// the physical memory range.
				// (A segment can be made of multiple sections and can for example contain
				// a .bss section after the .data in which case it would be unnecessary to
				// map the .bss part in the physical memory range)
				//
				// The new empty segment is however of the full size. Once the progbits
				// are copied over it duting startup, it will be equal to the one originally
				// defined in the ELF file.

				mem.map(segment.data(),
						physical_address,
						progbits_size, // only the progbits part is mapped
						segment_name);

				// Allocate a new empty segment
				program::memory_segment& relocation_segment = allocator.allocate_segment(prog, memory_size);

				// Map it in the virtual address space
				mem.map(relocation_segment.data(),
						virtual_address,
						memory_size, // the 'full' memory size is used here
						std::string("reloc of " + segment_name));

			} else {
				// Standard segment -> Map in virtual address space

				mem.map(segment.data(), virtual_address, memory_size, std::string(segment_name));
			}
		}

		return prog;
	}
};

} // namespace micromachine::system
