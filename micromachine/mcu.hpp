/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_MCU_HPP
#define MICROMACHINE_MCU_HPP

#include "cpu.hpp"
#include "exception_controller.hpp"
#include "exception_vector.hpp"
#include "memory/memory.hpp"
#include "nvic.hpp"
#include "registers/custom/generic_io_reg.hpp"
#include "registers/system_control/cpuid_reg.hpp"
#include "registers/system_control/shpr2_reg.hpp"
#include "registers/system_control/shpr3_reg.hpp"
#include "systick.hpp"

namespace micromachine::system {

class mcu {
private:
	nvic _nvic;
	shpr2_reg _shpr2_reg;
	shpr3_reg _shpr3_reg;
	cpuid_reg _cpuid_reg;
	generic_io_reg::callback_t _io_reg_callback;
	generic_io_reg _generic_io_reg;
	systick _systick;

	exception_vector _exception_vector;
	exception_controller _exception_controller;
	memory _memory;
	cpu _cpu;

	memory::system_control_register_map generate_system_control_register_map() {
		return {
			{cpuid_reg::CPUID, _cpuid_reg},
			{shpr2_reg::SHPR2, _shpr2_reg},
			{shpr3_reg::SHPR3, _shpr3_reg},
			{systick_control_reg::SYST_CSR, _systick.control_register()},
			{systick_control_reg::SYST_RVR, _systick.reload_value_register()},
			{systick_control_reg::SYST_CVR, _systick.current_value_register()},
			{systick_control_reg::SYST_CALIB, _systick.calib_value_register()},
			{generic_io_reg::GIO_IO, _generic_io_reg},
			{nvic::NVIC_ISER, _nvic.iser_reg()},
			{nvic::NVIC_ICER, _nvic.icer_reg()},
			{nvic::NVIC_ISPR, _nvic.ispr_reg()},
			{nvic::NVIC_ICPR, _nvic.icpr_reg()},
			{nvic::NVIC_IPR0, _nvic.priority_reg<0>()},
			{nvic::NVIC_IPR1, _nvic.priority_reg<1>()},
			{nvic::NVIC_IPR2, _nvic.priority_reg<2>()},
			{nvic::NVIC_IPR3, _nvic.priority_reg<3>()},
			{nvic::NVIC_IPR4, _nvic.priority_reg<4>()},
			{nvic::NVIC_IPR5, _nvic.priority_reg<5>()},
			{nvic::NVIC_IPR6, _nvic.priority_reg<6>()},
			{nvic::NVIC_IPR7, _nvic.priority_reg<7>()},
		};
	}

public:
	cpu& operator=(const cpu& other) = delete;

	mcu()
		: _generic_io_reg(_io_reg_callback)
		, _systick(_exception_controller)
		, _exception_vector(_nvic, _shpr2_reg, _shpr3_reg)
		, _exception_controller(_exception_vector)
		, _memory(_exception_controller, generate_system_control_register_map())
		, _cpu(_memory, _exception_vector, _exception_controller) {}

	mcu(const mcu& other)
		: _nvic(other._nvic)
		, _shpr2_reg(other._shpr2_reg)
		, _shpr3_reg(other._shpr3_reg)
		, _cpuid_reg(other._cpuid_reg)
		, _io_reg_callback(other._io_reg_callback)
		, _generic_io_reg(_io_reg_callback)
		, _systick(_exception_controller, other._systick)
		, _exception_vector(_nvic, _shpr2_reg, _shpr3_reg, other._exception_vector)
		, _exception_controller(_exception_vector)
		, _memory(_exception_controller, generate_system_control_register_map())
		, _cpu(_memory, _exception_vector, _exception_controller, other._cpu) {}

	const memory& get_memory() const {
		return _memory;
	}

	memory& get_memory() {
		return _memory;
	}

	const cpu& get_cpu() const {
		return _cpu;
	}

	cpu& get_cpu() {
		return _cpu;
	}

	const exception_vector& get_exception_vector() const {
		return _exception_vector;
	}

	void set_io_callback(generic_io_reg::callback_t callback) {
		_io_reg_callback = std::move(callback);
	}

	void reset(uint32_t program_entry_point) {
		_systick.reset();
		_shpr2_reg.reset();
		_shpr3_reg.reset();
		_nvic.reset();
		_exception_vector.reset();
		_cpu.reset(program_entry_point);
	}

	cpu::step_result step();
};

}

#endif // MICROMACHINE_MCU_HPP
