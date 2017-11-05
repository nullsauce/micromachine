/* Copyright (C) MindMaze SA - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of MindMaze SA.

*/

#ifndef MICROMACHINE_PC_REG_HPP
#define MICROMACHINE_PC_REG_HPP

#include "standard_reg.hpp"
#include "exec_mode_reg.hpp"
#include "exception_vector.hpp"

class pc_reg : public standard_reg {
public:

	using standard_reg::standard_reg;
	using standard_reg::operator=;

	bool _dirty_status;

	pc_reg(const exec_mode_reg& exec_mode_reg, exception_vector::bitref_t& hardfault_signal)
		: _exec_mode_reg(exec_mode_reg)
		, _hardfault_signal(hardfault_signal) {

	}

	void branch(word address) {

		// if EPSR.T == 0, a UsageFault('Invalid State')
		// is taken on the next instruction

		if(_exec_mode_reg.is_handler_mode() &&
			0b1111 == address.uint(28,4)) {
			// TODO ExceptionReturn
			precond_fail("ExceptionReturn unimplemented")
		}

		if(!address.bit(0)) {
			// Thumb bit not set, triggers a fault
			_hardfault_signal = true;
			fprintf(stderr, "PC: Thumb bit not set\n");
		}

		set(address);
	}

private:

	static const uint32_t MASK = binops::make_mask<1, 31>();

	void set(word word) override {
		_word = word;
		_dirty_status = true;
	}

	const word get() const override {
		word val = _word & MASK;
		return val;
	}

	const exec_mode_reg& 			_exec_mode_reg;
	exception_vector::bitref_t&	_hardfault_signal;
};


#endif //MICROMACHINE_PC_REG_HPP
