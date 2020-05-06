
//
// Created by fla on 04.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "registers/memory_mapped_reg.hpp"
#include "types/types.hpp"


namespace micromachine::system {

class interrupt_control_state_reg : public standard_reg {
private:
	//exception_controller& _exception_controller;

public:
	static constexpr uint32_t ICSR = 0xE000ED04;

	using nmi_pending_bit = bits<31>;
	using pensdv_pending_bit = bits<28>;
	using systick_pending_bit = bits<26>;

	/*
	interrupt_control_state_reg(exception_controller& exception_controller)
		: _exception_controller(exception_vector) {}

	interrupt_control_state_reg(exception_controller& exception_controller, const interrupt_control_state_reg& other)
		: standard_reg(other)
		, _exception_controller(exception_vector) {}*/

	const auto pendsv_pending() const {
		return pensdv_pending_bit::of(_word);
	}

	auto pendsv_pending() {
		return pensdv_pending_bit::of(_word);
	}

	const auto systick_pending() const {
		return systick_pending_bit::of(_word);
	}

	auto systick_pending() {
		return systick_pending_bit::of(_word);
	}

	const auto nmi_pending() const {
		return nmi_pending_bit::of(_word);
	}

	auto nmi_pending() {
		return nmi_pending_bit::of(_word);
	}

	void reset() override {
		_word = 0;
	}

private:
	void set(uint32_t word) override {
		_word = word;
	}

	uint32_t get() const override {
		return _word;
	}
};

} // namespace micromachine::system
