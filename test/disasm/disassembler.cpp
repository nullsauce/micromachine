/* Copyright (C) The Micromachine project - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of The Micromachine project.

*/

#include <gtest/gtest.h>
#include "disasm.hpp"
#include <fstream>

TEST(Dissasembler, TestVersusObjdumpOutput) {

	const char* path = SOURCE_DIR"/instructions2.txt";
	std::ifstream instructions_file(path);
	std::string line;
	while(std::getline(instructions_file, line) ) {

		// 9 first characters are either a single or double instruction
		/*
		 *d45c       bmi.n 0xc2
		 *f110 1319  adds.w r3, r0, #1638425 ; 0x190019
		 */
		std::string instr_str = line.substr(0, 9);
		std::string disam_ref = line.substr(11);

		auto semicolon_pos = line.find(";");
		if(std::string::npos != semicolon_pos) {
			disam_ref = disam_ref.substr(0, semicolon_pos);
		}
		uint16_t first_instruction = 0;
		uint16_t second_instruction = 0;
		int num_decoded = std::sscanf(instr_str.data(), "%hx %hx", &first_instruction, &second_instruction);
		fprintf(stderr, "%d : %04hx %04hx -> %s\n", num_decoded, first_instruction, second_instruction, disam_ref.c_str());
		std::string disassembled = disasm::disassemble_instruction(instruction_pair(first_instruction, second_instruction));

		EXPECT_STREQ(disassembled.c_str(), disam_ref.c_str());
	}
}