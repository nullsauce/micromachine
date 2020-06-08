#pragma once

#include "CodeGenerator.hpp"
#include "instruction/instructions.hpp"

namespace micromachine::testing {

class MiniAssembler {
private:
	CodeGenerator& _code_gen;

public:
	MiniAssembler(CodeGenerator& code_gen)
		: _code_gen(code_gen)

	{}

	MiniAssembler& seek_to(uint32_t address) {
		_code_gen.setWriteAddress(address);
		return *this;
	}

	MiniAssembler& emit_nop() {
		_code_gen.emit16bit("1011111100000000");
		return *this;
	}

	MiniAssembler& emit_unconditional_branch(int16_t offset) {
		micromachine::system::unconditional_branch b;
		b.set_offset(offset);
		_code_gen.write(b.value());
		return *this;
	}

	MiniAssembler& emit_bl(int16_t offset) {
		micromachine::system::unconditional_branch b;
		b.set_offset(offset);
		_code_gen.write(b.value());
		return *this;
	}

	MiniAssembler& emit_bx_lr() {
		_code_gen.write(0b0100011101110000);
		return *this;
	}

	MiniAssembler& emit_pop_pc() {
		_code_gen.write(0b1011110100000000);
		return *this;
	}

	MiniAssembler& emit_push_lr() {
		_code_gen.write(0b1011010100000000);
		return *this;
	}
};

}
