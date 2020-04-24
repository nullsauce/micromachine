#pragma once

#include "memory/memory.hpp"

#include <cstdarg>

namespace micromachine::testing {

class CodeGenerator {
private:

	uint32_t _writeAddress;
	uint32_t _baseAddress;
	micromachine::system::memory* _mem;

public:
	CodeGenerator();

	void setMem(micromachine::system::memory* mem);
	void setWriteAddress(uint32_t address);
	uint32_t writeAddress() const ;
	void emit16bit(const char* encoding, ...);
	void emit16bitVaList(const char* encoding, va_list args);
	void emit32bit(const char* encoding1, const char* encoding2, ...);
	void emit32bitVaList(const char* encoding1, const char* encoding2, va_list args);
	void write(uint16_t instruction);

	void emitNop() {
		emit16bit("1011111100000000");
	}

	void emitAdc(micromachine::system::reg_idx m, micromachine::system::reg_idx d) {
		emit16bit("0100000101mmmddd", m, d);
	}

	void emitAddImmT1(micromachine::system::reg_idx imm, micromachine::system::reg_idx n, micromachine::system::reg_idx d) {
		emit16bit("0001110iiinnnddd", imm, n, d);
	}

	void emitAddImmT2(micromachine::system::reg_idx d, micromachine::system::reg_idx imm) {
		emit16bit("00110dddiiiiiiii", d, imm);
	}

	void emitAddT1(micromachine::system::reg_idx m, micromachine::system::reg_idx n, micromachine::system::reg_idx d) {
		emit16bit("0001100mmmnnnddd", m, n, d);
	}

	void emitAddT2(micromachine::system::reg_idx m, micromachine::system::reg_idx d) {
		emit16bit("01000100dmmmmddd", m, d);
	}

private:
	static uint16_t assemble_instruction(const char* encoding, va_list valist);
};

} // namespace micromachine::system::helpers

