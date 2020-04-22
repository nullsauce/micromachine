/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_SPECIAL_REGISTERS_HPP
#define MICROMACHINE_SPECIAL_REGISTERS_HPP

#include "registers/apsr_reg.hpp"
#include "registers/control_reg.hpp"
#include "registers/epsr_reg.hpp"
#include "registers/ipsr_reg.hpp"
#include "registers/primask_reg.hpp"

class special_registers {
private:
	uint32_t _xpsr_register;
	apsr_reg _app_status_register;
	ipsr_reg _interrupt_status_register;
	epsr_reg _execution_status_register;
	control_reg _control_register;
	primask_reg _primask_register;

public:
	special_registers()
		: _app_status_register(_xpsr_register)
		, _interrupt_status_register(_xpsr_register)
		, _execution_status_register(_xpsr_register) {}

	special_registers(const special_registers& other)
		: _xpsr_register(other._xpsr_register)
		, _app_status_register(_xpsr_register)
		, _interrupt_status_register(_xpsr_register)
		, _execution_status_register(_xpsr_register)
		, _control_register(other._control_register)
		, _primask_register(other._primask_register) {}

	uint32_t& xpsr_register() {
		return _xpsr_register;
	}

	const uint32_t& xpsr_register() const {
		return _xpsr_register;
	}

	apsr_reg& app_status_register() {
		return _app_status_register;
	}

	const apsr_reg& app_status_register() const {
		return _app_status_register;
	}

	ipsr_reg& interrupt_status_register() {
		return _interrupt_status_register;
	}

	const ipsr_reg& interrupt_status_register() const {
		return _interrupt_status_register;
	}

	epsr_reg& execution_status_register() {
		return _execution_status_register;
	}

	const epsr_reg& execution_status_register() const {
		return _execution_status_register;
	}

	const control_reg& control_register() const {
		return _control_register;
	}

	control_reg& control_register() {
		return _control_register;
	}

	primask_reg& primask_register() {
		return _primask_register;
	}

	const primask_reg& primask_register() const {
		return _primask_register;
	}
};

#endif // MICROMACHINE_SPECIAL_REGISTERS_HPP
