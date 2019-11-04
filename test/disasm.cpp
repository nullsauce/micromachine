//
// Created by fla
//

#include <gtest/gtest.h>
#include "nvic.hpp"
#include "disasm.hpp"

TEST(Disasm, DisasmGenerate)
{
	for(int i = 0; i < 65536; i++) {
		instruction_pair instr((uint16_t)i, 0);
		std::string str = disasm::disassemble_instruction(instr, 64);
		fprintf(stderr, "%04x %s\n", (uint16_t)instr.first(), str.c_str());
	}
}