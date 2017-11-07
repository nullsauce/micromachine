#include <gtest/gtest.h>
#include "disasm.hpp"
#include <fstream>

TEST(Dissasembler, TestVersusObjdumpOutput) {

	const char* path = SOURCE_DIR"/medium2.txt";
	std::ifstream instructions_file(path);
	std::string line;
	//word address = 0;
	while(std::getline(instructions_file, line) ) {
		if(line.length() < 17) continue;
		// 9 first characters are either a single or double instruction
		/*
		 *d45c       bmi.n 0xc2
		 *f110 1319  adds.w r3, r0, #1638425 ; 0x190019
		 */
		std::string instr_str = line;
		std::string disam_ref = line.substr(17);

		uint16_t first_instruction = 0;
		uint16_t second_instruction = 0;
		int32_t address = -1;
		int num_decoded = std::sscanf(instr_str.data(), "%x: %hx %hx", &address, &first_instruction, &second_instruction);
		ASSERT_NE(-1, address);
		fprintf(stderr, "[%04d] %d : %04hx %04hx -> %s\n", address, num_decoded, first_instruction, second_instruction, disam_ref.c_str());
		fprintf(stderr, "%s %s\n", ((halfword)first_instruction).to_string().c_str(), ((halfword)second_instruction).to_string().c_str());
		std::string disassembled = disasm::disassemble_instruction(instruction_pair(first_instruction, second_instruction), address);
		ASSERT_STREQ(disassembled.c_str(), disam_ref.c_str());
		//address = address + (num_decoded*sizeof(uint16_t));
	}
}