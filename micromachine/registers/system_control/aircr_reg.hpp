
//
// Created by fla on 04.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "exception/exception_controller.hpp"
#include "registers/standard_reg.hpp"
#include "types/types.hpp"
#include "utils/signal.hpp"

namespace micromachine::system {

class interrupt_and_reset_control_reg : public standard_reg {
private:
	exception_controller& _exception_controller;
	signal& _reset_signal;

public:
	static constexpr uint32_t AIRCR = 0xE000ED0C;
	static constexpr uint32_t RESET_VALUE = 0x05FA0000;

	using clear_exception_state_bit = bits<1>;
	using reset_request_bit = bits<2>;
	using endianess_bit = bits<15>;

	interrupt_and_reset_control_reg(exception_controller& exception_controller, signal& reset_signal)
		: standard_reg(RESET_VALUE)
		, _exception_controller(exception_controller)
		, _reset_signal(reset_signal)
	{}

	void reset() override {
		_word = RESET_VALUE;
	}

	auto is_big_endian() const {
		return bits_ref<endianess_bit>();
	}

private:
	void set(uint32_t word) override {
		if(clear_exception_state_bit::of(word)) {
			_exception_controller.reset();
		}

		if(reset_request_bit::of(word)) {
			_reset_signal.set();
		}
	}

	uint32_t get() const override {
		return _word;
	}
};

} // namespace micromachine::system
