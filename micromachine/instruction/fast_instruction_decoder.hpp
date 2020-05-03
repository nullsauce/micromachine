
//
// Created by fla on 03.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "instruction_decoder.hpp"
#include "jump_table_builder.hpp"

namespace micromachine::system {

class fast_instruction_decoder : public instruction_decoder {
private:
	std::array<std::function<void()>, 0x10000> _jump_table;
	jumptable16_builder<fast_instruction_decoder> _jump_table_builder;

public:
	fast_instruction_decoder()
		: _jump_table_builder(this) {

		for(uint32_t i = 0; i < 0x10000; i++) {
			uint16_t instruction = (uint16_t)i;
			_jump_table_builder.decode_16bit_instruction(instruction);
			auto func = _jump_table_builder.function();
			if(!func) {
				micromachine_fail("dafuq");
			}
			_jump_table[instruction] = std::move(func);
		}
	}

	void decode_instruction(const instruction_pair instruction_pair) {
		if(is_32bit_thumb_encoding(instruction_pair.first())) {
			decode_32bit_instruction(instruction_pair);
		} else {
			_jump_table[instruction_pair.first()]();
		}
	}
};
}
