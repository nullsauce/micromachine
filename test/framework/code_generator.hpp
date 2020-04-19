#ifndef MICROMACHINE_EMU_CODE_GENERATOR_HPP
#define MICROMACHINE_EMU_CODE_GENERATOR_HPP

#include "memory/memory.hpp"

#include <cstdarg>

class code_generator {
private:

	uint32_t _write_address;
	uint32_t _base_address;
	memory* _mem;

public:
	code_generator();
	void set_mem(memory* mem);
	void set_write_address(uint32_t address);
	void emit_ins16(const char* encoding, ...);
	void emit_ins32(const char* encoding1, const char* encoding2, ...);
	void write(uint16_t instruction);

	void emit_adc(reg_idx m, reg_idx d) {
		emit_ins16("0100000101mmmddd", m, d);
	}

	void emit_add_imm_t1(reg_idx imm, reg_idx n, reg_idx d) {
		emit_ins16("0001110iiinnnddd", imm, n, d);
	}

	void emit_add_imm_t2(reg_idx d, reg_idx imm) {
		emit_ins16("00110dddiiiiiiii", d, imm);
	}

	void emit_add_t1(reg_idx m, reg_idx n, reg_idx d) {
		emit_ins16("0001100mmmnnnddd", m, n, d);
	}

	void emit_add_t2(reg_idx m, reg_idx d) {
		emit_ins16("01000100dmmmmddd", m, d);
	}


private:
	static uint16_t assemble_instruction(const char* encoding, va_list valist);
};


#endif //MICROMACHINE_EMU_CODE_GENERATOR_HPP
