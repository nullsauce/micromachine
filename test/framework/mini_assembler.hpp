/* Copyright (C) MindMaze SA - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of MindMaze SA.

*/

#ifndef MICROMACHINE_EMU_MINI_ASSEMBLER_HPP
#define MICROMACHINE_EMU_MINI_ASSEMBLER_HPP

#include "code_generator.hpp"
#include "instructions.hpp"

class mini_assembler {
private:
	code_generator& _code_gen;

public:
	mini_assembler(code_generator& code_gen)
		: _code_gen(code_gen)

	{}

	void seek_to(uint32_t address) {
		_code_gen.set_write_address(address);
	}

	mini_assembler& emit_nop() {
		_code_gen.emit_ins16("1011111100000000");
		return *this;
	}

	mini_assembler& emit_unconditional_branch(int16_t offset) {
		unconditional_branch b;
		b.set_offset(offset);
		_code_gen.write(b.value());
		return *this;
	}

	mini_assembler& emit_bl(int16_t offset) {
		unconditional_branch b;
		b.set_offset(offset);
		_code_gen.write(b.value());
		return *this;
	}

};

#endif //MICROMACHINE_EMU_MINI_ASSEMBLER_HPP
