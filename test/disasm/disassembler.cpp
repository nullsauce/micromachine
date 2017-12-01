#include <gtest/gtest.h>
#include "disasm.hpp"
#include <fstream>
#include <inttypes.h>
#include <capstone/capstone.h>


bool read_file(const std::string& path, std::vector<uint8_t>& buffer) {
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	if(size == -1) return false;
	file.seekg(0, std::ios::beg);
	buffer.clear();
	buffer.resize(size);
	file.read((char *)buffer.data(), size);
	return true;
}


TEST(Dissasembler, TestVersusCapstone) {

	const char* path = SOURCE_DIR"/random.bin";
	std::vector<uint8_t> data;
	ASSERT_TRUE(read_file(path, data));

	csh handle;

	size_t count;

	if (cs_open(CS_ARCH_ARM, CS_MODE_THUMB, &handle) != CS_ERR_OK)
		return;

	cs_insn* insn = nullptr;
	uint32_t address = 0;
	uint8_t* next_data = data.data();
	do {
		if(insn) free(insn);
		get_next:
		count = cs_disasm(handle, next_data, data.size(), 0, 2, &insn);
		if(nullptr == insn) {
			next_data++;
			goto get_next;
		}
		for (size_t j = 0; j < count; j++) {
			address = next_data - data.data();
			printf("0x%" PRIx64 ":\t%s\t\t%s\n", address, insn[j].mnemonic, insn[j].op_str);
			uint16_t* instruction = (uint16_t*)&data[address];
			uint16_t first_instruction = instruction[0];
			uint16_t second_instruction = insn[j].size == 4 ? instruction[1] : 0;
			std::string disassembled = disasm::disassemble_instruction(instruction_pair(first_instruction, second_instruction), address-8);
			std::string reference = std::string(insn[j].mnemonic) + " " + std::string(insn[j].op_str);
			EXPECT_STREQ(disassembled.c_str(), reference.c_str());
			next_data += insn[j].size;

		}


	} while(count > 0);

}
/*
TEST(Dissasembler, TestVersusObjdumpOutput) {

	const char* path = SOURCE_DIR"/medium.txt";
	std::ifstream instructions_file(path);
	std::string line;

	while(std::getline(instructions_file, line) ) {
		if(line.length() < 17) continue;
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
}*/