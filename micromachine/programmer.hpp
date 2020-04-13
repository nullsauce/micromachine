/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_PROGRAMMER_HPP
#define MICROMACHINE_EMU_PROGRAMMER_HPP

#include "memory/memory.hpp"
#include "elfio/elfio.hpp"

namespace {

void hexdump(const void* ptr, int buflen) {
	unsigned char *buf = (unsigned char*)ptr;
	int i, j;
	fprintf(stdout,"\npacket of %d bytes\n", buflen);
	for (i=0; i<buflen; i+=16) {
		fprintf(stdout, "%06x: ", i);
		for (j=0; j<16; j++) {
			if (i+j < buflen) {
				fprintf(stdout, "%02x ", buf[i+j]);
			} else {
				fprintf(stdout, "   ");
			}
		}
		printf(" ");
		for (j=0; j<16; j++) {
			if (i+j < buflen) {
				fprintf(stdout, "%c", isprint(buf[i+j]) ? buf[i+j] : '.');
			}
		}
		fprintf(stdout, "\n");
	}
}

}

class programmer {
private:
	struct membuf : std::streambuf {
		membuf(char* begin, char* end) {
			this->setg(begin, begin, end);
		}
	};

public:

	class program {
	private:
		uint8_t* const _data;
		const  size_t _size;

	public:
		using ptr = std::shared_ptr<program>;
		using memory_segment = std::vector<uint8_t>;

		program(size_t size)
			: _data((uint8_t*)calloc(1, size))
			, _size(size)
		{}

		memory_segment& allocate_segment(size_t size) {
			_memory.emplace_back(size, (uint8_t)0);
			return _memory.at(_memory.size()-1);
		}

		bool is_null() const {
			return _memory.empty();
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

		uint8_t* data() {
			return _data;
		}

		const uint8_t* data() const {
			return _data;
		}

		size_t size() const {
			return _size;
		}

		membuf memory_buffer() const {
			return membuf((char*)data(), (char*)data() + size());
		}

	private:
		std::vector<memory_segment> _memory;
		uint32_t _entry_point = 0;
		std::string _path;
	};


	static program::ptr load_elf(const std::string &path, memory& mem) {

		std::ifstream ifs(path, std::ios::binary|std::ios::ate);
		if(!ifs) {
			fprintf(stderr, "Failed to open file path=%s\n", path.c_str());
			return nullptr;
		}

		std::ifstream::pos_type size = ifs.tellg();
		program::ptr prog = std::make_shared<program>(size);
		prog->set_path(path);

		ifs.seekg(0, std::ios::beg);
		ifs.read((char*)prog->data(), prog->size());

		//membuf memory = prog->memory_buffer();
		//std::istream memory_stream(&memory);
		std::istringstream ss;
		ss.rdbuf()->pubsetbuf((char*)prog->data(),prog->size());

		ELFIO::elfio elfReader;
		if (!elfReader.load(ss)) {
			return prog;
		}

		if (elfReader.get_class() != ELFCLASS32) {
			return prog;
		}

		if (elfReader.get_encoding() != ELFDATA2LSB) {
			return prog;
		}

		uint32_t entryPoint = (uint32_t)elfReader.get_entry();
		fprintf(stderr, "PC will be set to %08X\n", entryPoint);

		prog->set_entry_point(entryPoint);


		/* Process each segment defined in the ELF file
		 * and prepare the appropriate memory mappings for
		 * the executable to run properly.
		 *
		 * The segments containing pre-initialized data
		 * (also called progbits in this context) will have their
		 * data loaded from the ELF file before being mapped.
		 *
		 * Relocations
		 * A segment is considered relocatable if it has pre-initialized data
		 * (aka: progbits) AND if its virtual address is different that its physical
		 * address. It this case, the progbits of the segment are mapped at the
		 * _physical_ address and an additional new segment of the appropriate
		 * _memory size_ is created and mapped (without any data) at the _virtual_ address.
		 *
		 * Segments with different physical and virtual addresses who don't have
		 * progbits are simply mapped at the virtual address.
		 *
		 * The runtime is responsible for relocating a segment at startup by coyping the
		 * progbits from the physical address to the memory address.
		 */
		for (const ELFIO::segment* elf_segment : elfReader.segments) {

			const uint32_t virtual_address = (uint32_t)elf_segment->get_virtual_address();
			const uint32_t physical_address = (uint32_t)elf_segment->get_physical_address();
			const uint32_t memory_size = (uint32_t)elf_segment->get_memory_size();
			const uint32_t progbits_size = (uint32_t)elf_segment->get_file_size();
			const uint32_t segment_index = elf_segment->get_index();
			const bool has_progbits = progbits_size != 0;
			const bool requires_relocation = has_progbits && virtual_address != physical_address;

			fprintf(stderr, "segment [%02i] : virtual=%08x physical=%08x map=%s size=%08x progbits=%s "
				   "relocation=%s\n"
				, elf_segment->get_index()
				, virtual_address
				, physical_address
				, requires_relocation ? "phys" : "virt"
				, memory_size
				, has_progbits ? "yes" : "no"
				, requires_relocation ? "yes" : "no "
			);

			program::memory_segment& segment = prog->allocate_segment(memory_size);

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

				mem.map(
					segment.data(),
					physical_address,
					progbits_size, // only the progbits part is mapped
					std::string("segment "+ std::to_string(elf_segment->get_index()))
				);

				// Allocate a new empty segment
				program::memory_segment& relocation_segment = prog->allocate_segment(memory_size);

				// Map it in the virtual address space
				mem.map(
					relocation_segment.data(),
					virtual_address,
					memory_size // the 'full' memory size is used here
				);

			} else {
				// Standard segment -> Map in virtual address space

				mem.map(
					segment.data(),
					virtual_address,
					memory_size,
					std::string("segment "+ std::to_string(elf_segment->get_index()))
				);
			}
		}

		/*
		for (int i = 0; i < sec_num; ++i) {
			ELFIO::section *section = elfReader.sections[i];

			if(section->get_flags() & (SHF_ALLOC | SHF_EXECINSTR)) {

				if(0 == section->get_size()) {
					fprintf(stderr, "Skipping empty section %s\n", section->get_name().c_str());
					continue;
				}

				fprintf(stderr, "Creating memory section for %s\n", section->get_name().c_str());

				program::memory_segment& chunk = prog->allocate_segment(section->get_size());

				// should we copy data to this section ?
				if(section->get_type() & SHT_PROGBITS) {
					fprintf(stderr, "Loading %zu bytes into section %s\n"
						, section->get_size()
						, section->get_name().c_str()
					);
					memcpy(chunk.data(), section->get_data(), section->get_size());
				}

				fprintf(stderr, "Mapping section %s to %08lx (size = %08lx)\n"
					, section->get_name().c_str()
					, section->get_address()
					, section->get_size()
				);

				mem.map(chunk.data(), section->get_address(), section->get_size(), section->get_name());
			} else {
				fprintf(stderr, "Ignored section %s\n", section->get_name().c_str());
			}
		}*/

		return prog;
	}


};

#endif //MICROMACHINE_EMU_PROGRAMMER_HPP
