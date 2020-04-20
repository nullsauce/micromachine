
#include "code_generator.hpp"
#include <cstring>

code_generator::code_generator()
	: _write_address(0)
	, _base_address(0)
	, _mem(nullptr)
{

}

void code_generator::set_mem(memory* mem) {
	_mem = mem;
}

void code_generator::set_write_address(uint32_t address) {
	_write_address = address;
}

uint32_t code_generator::write_address() const {
	return _write_address;
}

void code_generator::emit_ins16(const char* encoding, ...) {
	va_list valist;
	va_start(valist, encoding);
	uint16_t instruction = assemble_instruction(encoding, valist);
	va_end(valist);
	write(instruction);
}

void code_generator::emit_ins32(const char* encoding1, const char* encoding2, ...) {
	va_list valist;
	va_start(valist, encoding2);
	uint16_t first = assemble_instruction(encoding1, valist);
	uint16_t second = assemble_instruction(encoding2, valist);
	va_end(valist);
	write(first);
	write(second);
}

void code_generator::write(uint16_t instruction) {
	precond(_mem != nullptr, "mem can't be null");
	_mem->write16(_write_address, instruction);
	_write_address += sizeof(uint16_t);
}

uint16_t code_generator::assemble_instruction(const char* encoding, va_list valist) {
	uint16_t instr = 0;
	size_t i = 0;
	char last = '\0';
	const char *p;

	struct Field
	{
		uint32_t value;
		char c;
	} fields[6];

	assert (16 == strlen(encoding));
	memset(fields, 0, sizeof(fields));

	// Go through encoding from left to right and find all fields to be inserted.
	p = encoding;
	while (*p) {
		char c = *p++;

		if (c != '1' && c != '0' && c != last) {
			// Determine if we already saw this field earlier.
			bool found = false;
			for (size_t j = 0; j < i; j++) {
				if (fields[j].c == c)
					found = true;
			}

			// If this is the first time we have seen the field, then save its value in fields array.
			if (!found) {
				assert (i < sizeof(fields) / sizeof(fields[0]));

				fields[i].value = va_arg(valist, uint32_t);
				fields[i].c = c;
				last = c;
				i++;
			}
		}
	}

	// Go through encoding again from right to left and insert field bits.
	p = encoding + 15;
	while (p >= encoding) {
		char c = *p--;

		instr >>= 1;

		if (c == '1') {
			instr |= (1 << 15);
		} else if (c == '0') {
			instr |= (0 << 15);
		} else {
			size_t j;
			for (j = 0; j < i; j++) {
				if (fields[j].c == c)
					break;
			}
			assert (j != i);

			instr |= (fields[j].value & 1) << 15;
			fields[j].value >>= 1;
		}
	}

	return instr;
}
