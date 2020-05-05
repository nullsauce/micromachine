
//
// Created by fla on 04.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "registers/memory_mapped_reg.hpp"
#include "types/types.hpp"
#include "exception/exception_vector.hpp"

namespace micromachine::system {

class interrupt_control_state_reg : public memory_mapped_reg {
private:
	exception_vector& _exception_vector;

public:
	static constexpr uint32_t ICSR = 0xE000ED04;

	using clear_exception_state_bit = bits<1>;
	using reset_request_bit = bits<2>;
	using endianess_bit = bits<15>;

	interrupt_control_state_reg(exception_vector& exception_vector)
		: _exception_vector(exception_vector)
	{}

	void reset() override {
		_word = 0;
	}

private:
	void set(uint32_t word) override {

	}

	uint32_t get() const override {
		return _word;
	}
};

} // namespace micromachine::system
