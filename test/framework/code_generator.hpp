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

private:
	static uint16_t assemble_instruction(const char* encoding, va_list valist);
};


#endif //MICROMACHINE_EMU_CODE_GENERATOR_HPP
