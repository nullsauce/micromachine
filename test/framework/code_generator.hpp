#ifndef MICROMACHINE_EMU_CODE_GENERATOR_HPP
#define MICROMACHINE_EMU_CODE_GENERATOR_HPP

#include "memory.hpp"

#include <cstdarg>

class code_generator {
private:

	uint32_t _write_pos;
	uint32_t _base_address;
	memory* _mem;

public:

	code_generator();
	void set_mem(memory* mem);
	void set_base_address(uint32_t address);
	void set_write_pos(uint32_t pos);
	void emit_ins16(const char* encoding, ...);
	void emit_ins32(const char* encoding1, const char* encoding2, ...);

private:
	void write(uint16_t instruction);
	static uint16_t assemble_instruction(const char* encoding, va_list valist);
};


#endif //MICROMACHINE_EMU_CODE_GENERATOR_HPP
