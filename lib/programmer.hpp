/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_PROGRAMMER_HPP
#define MICROMACHINE_EMU_PROGRAMMER_HPP

#include "memory/memory.hpp"
#include "elfio/elfio.hpp"

class programmer {
public:

	class program {
	public:
		using ptr = std::shared_ptr<program>;
		using memory_chunk = std::vector<uint8_t>;
		memory_chunk& allocate_chunk(size_t size) {
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
	private:
		std::vector<memory_chunk> _memory;
		uint32_t _entry_point = 0;
	};


	static program::ptr load_elf(const std::string &path, memory& mem) {
		program::ptr prog = std::make_shared<program>();
		ELFIO::elfio elfReader;
		if (!elfReader.load(path)) {
			return prog;
		}

		if (elfReader.get_class() != ELFCLASS32) {
			return prog;
		}

		if (elfReader.get_encoding() != ELFDATA2LSB) {
			return prog;
		}


		ELFIO::Elf_Half sec_num = elfReader.sections.size();

		uint32_t entryPoint = (uint32_t)elfReader.get_entry();
		fprintf(stderr, "PC will be set to %08X\n", entryPoint);

		prog->set_entry_point(entryPoint);

		//std::cout << "Number of sections: " << sec_num << std::endl;
		for (int i = 0; i < sec_num; ++i) {
			ELFIO::section *section = elfReader.sections[i];

			if(section->get_flags() & (SHF_ALLOC | SHF_EXECINSTR)) {

				if(0 == section->get_size()) {
					fprintf(stderr, "Skipping empty section %s\n", section->get_name().c_str());
					continue;
				}

				fprintf(stderr, "Creating memory section for %s\n", section->get_name().c_str());

				program::memory_chunk& chunk = prog->allocate_chunk(section->get_size());

				// should we copy data to this section ?
				if(section->get_type() & SHT_PROGBITS) {
					memcpy(chunk.data(), section->get_data(), section->get_size());
				}
				mem.map(chunk.data(), section->get_address(), section->get_size(), section->get_name());
			}
		}

		return prog;
	}


};

#endif //MICROMACHINE_EMU_PROGRAMMER_HPP
