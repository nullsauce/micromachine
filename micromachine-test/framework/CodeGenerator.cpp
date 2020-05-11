
#include "CodeGenerator.hpp"
#include "helpers/check.hpp"
#include <cstring>

namespace micromachine::testing {

using namespace micromachine::system;

CodeGenerator::CodeGenerator()
	: _writeAddress(0)
	, _baseAddress(0)
	, _mem(nullptr) {}

void CodeGenerator::setMem(memory* mem) {
	_mem = mem;
}

void CodeGenerator::setWriteAddress(uint32_t address) {
	_writeAddress = address;
}

uint32_t CodeGenerator::writeAddress() const {
	return _writeAddress;
}

void CodeGenerator::emit16bit(const char* encoding, ...) {
	va_list valist;
	va_start(valist, encoding);
	uint16_t instruction = assemble_instruction(encoding, valist);
	va_end(valist);
	write(instruction);
}

void CodeGenerator::emit16bitVaList(const char* encoding, va_list args) {
	uint16_t instruction = assemble_instruction(encoding, args);
	write(instruction);
}

void CodeGenerator::emit32bit(const char* encoding1, const char* encoding2, ...) {
	va_list valist;
	va_start(valist, encoding2);
	emit32bitVaList(encoding1, encoding2, valist);
	va_end(valist);
}

void CodeGenerator::emit32bitVaList(const char* encoding1, const char* encoding2, va_list args) {
	write(assemble_instruction(encoding1, args));
	write(assemble_instruction(encoding2, args));
}

void CodeGenerator::write(uint16_t instruction) {
	micromachine_check(_mem != nullptr, "mem can't be null");
	_mem->write16(_writeAddress, instruction);
	_writeAddress += sizeof(uint16_t);
}

uint16_t CodeGenerator::assemble_instruction(const char* encoding, va_list valist) {
	uint16_t instr = 0;
	size_t i = 0;
	char last = '\0';
	const char* p;

	struct Field {
		uint32_t value;
		char c;
	} fields[6];

	micromachine_check(16 == strlen(encoding), "instruction template length must be 16");
	memset(fields, 0, sizeof(fields));

	// Go through encoding from left to right and find all fields to be inserted.
	p = encoding;
	while(*p) {
		char c = *p++;

		if(c != '1' && c != '0' && c != last) {
			// Determine if we already saw this field earlier.
			bool found = false;
			for(size_t j = 0; j < i; j++) {
				if(fields[j].c == c)
					found = true;
			}

			// If this is the first time we have seen the field, then save its value in fields array.
			if(!found) {
				micromachine_check(i < sizeof(fields) / sizeof(fields[0]));

				fields[i].value = va_arg(valist, uint32_t);
				fields[i].c = c;
				last = c;
				i++;
			}
		}
	}

	// Go through encoding again from right to left and insert field bits.
	p = encoding + 15;
	while(p >= encoding) {
		char c = *p--;

		instr >>= 1;

		if(c == '1') {
			instr |= (1 << 15);
		} else if(c == '0') {
			instr |= (0 << 15);
		} else {
			size_t j;
			for(j = 0; j < i; j++) {
				if(fields[j].c == c)
					break;
			}
			micromachine_check(j != i);

			instr |= (fields[j].value & 1) << 15;
			fields[j].value >>= 1;
		}
	}

	return instr;
}

}; // namespace micromachine::testing
