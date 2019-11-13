/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_PROGRAMMER_HPP
#define MICROMACHINE_EMU_PROGRAMMER_HPP

#include "memory/cpu.hpp"
#include "elfio/elfio.hpp"

class programmer {
public:

	static bool

	static bool cpu::load_elf(const std::string &path, cpu& mem) {
		ELFIO::elfio elfReader;
		if (!elfReader.load(path)) {
			return false;
		}

		if (elfReader.get_class() != ELFCLASS32) {
			return false;
		}

		if (elfReader.get_encoding() != ELFDATA2LSB) {
			return false;
		}

		ELFIO::Elf_Half sec_num = elfReader.sections.size();

		uint32_t entryPoint = (uint32_t)elfReader.get_entry();
		fprintf(stderr, "PC will be set to %08X\n", entryPoint);

		_initial_pc = entryPoint;

		//std::cout << "Number of sections: " << sec_num << std::endl;
		for (int i = 0; i < sec_num; ++i) {
			ELFIO::section *section = elfReader.sections[i];

			if(section->get_flags() & (SHF_ALLOC | SHF_EXECINSTR)) {

				if(0 == section->get_size()) {
					fprintf(stderr, "Skipping empty section %s\n", section->get_name().c_str());
					continue;
				}

				fprintf(stderr, "Creating memory section for %s\n", section->get_name().c_str());

				uint8_t* data = (uint8_t*)calloc(1, section->get_size());

				// should we copy data to this section ?
				if(section->get_type() & SHT_PROGBITS) {
					memcpy(data, section->get_data(), section->get_size());
				}
				cpu.mem().map(data, section->get_address(), section->get_size(), section->get_name());
			}
		}

		return true;
	}


};

#endif //MICROMACHINE_EMU_PROGRAMMER_HPP
