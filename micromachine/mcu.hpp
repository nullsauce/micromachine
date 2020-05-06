/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include "control_signals.hpp"
#include "cpu.hpp"
#include "exception/exception_vector_controller.hpp"
#include "memory/memory.hpp"
#include "nvic.hpp"
#include "peripherals/usart/usart_controller.hpp"
#include "registers/custom/generic_io_reg.hpp"
#include "registers/system_control/config_and_control_reg.hpp"
#include "registers/system_control/cpuid_reg.hpp"
#include "registers/system_control/interrupt_and_reset_control_reg.hpp"
#include "registers/system_control/interrupt_control_state_reg.hpp"
#include "registers/system_control/shpr2_reg.hpp"
#include "registers/system_control/shpr3_reg.hpp"
#include "registers/system_control/vtable_offset_reg.hpp"
#include "systick.hpp"

namespace micromachine::system {

class mcu {
public:
	enum class step_result {
		OK,
		HALT,
	};

private:
	nvic _nvic;
	shpr2_reg _shpr2_reg;
	shpr3_reg _shpr3_reg;
	exception_vector_controller _exception_controller;

	control_signals _control_signals;
	memory _memory;
	cpu _cpu;

	systick _systick;
	generic_io_reg::callback_t _io_reg_callback;
	generic_io_reg _generic_io_reg;
	cpuid_reg _cpuid_reg;
	config_and_control_reg _ccr_reg;
	interrupt_and_reset_control_reg _aircr;
	vtable_offset_reg _vtable_offset_reg;
	interrupt_control_state_reg _icsr_reg;

	usart_controller _usart_controller;

	uint32_t _previously_used_entrypoint;

	register_map::map generate_system_control_register_map() {
		return {
			{cpuid_reg::CPUID, _cpuid_reg},
			{shpr2_reg::SHPR2, _shpr2_reg},
			{shpr3_reg::SHPR3, _shpr3_reg},
			{interrupt_control_state_reg::ICSR, _icsr_reg},
			{vtable_offset_reg::VTOR, _aircr},
			{interrupt_and_reset_control_reg::AIRCR, _aircr},
			{config_and_control_reg::CCR, _ccr_reg},
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
			{usart_controller::USART_CR1, _usart_controller.control_register()},
			{usart_controller::USART_ISR, _usart_controller.interrupt_status_register()},
			{usart_controller::USART_ICR, _usart_controller.interrupt_clear_register()},
			{usart_controller::USART_RX, _usart_controller.rx_register()},
			{usart_controller::USART_TX, _usart_controller.tx_register()},
		};
	}

public:
	cpu& operator=(const cpu& other) = delete;

	mcu()
		: _exception_controller(_nvic, _shpr2_reg, _shpr3_reg, _icsr_reg)
		, _memory(_exception_controller, generate_system_control_register_map())
		, _cpu(_memory, _exception_controller, _control_signals)
		, _systick(_exception_controller)
		, _generic_io_reg(_io_reg_callback)
		, _aircr(_exception_controller, _control_signals.reset)
		, _icsr_reg()
		, _usart_controller(_exception_controller, exception::EXTI_00)
		, _previously_used_entrypoint(0) {}

	mcu(const mcu& other)
		: _nvic(other._nvic)
		, _shpr2_reg(other._shpr2_reg)
		, _shpr3_reg(other._shpr3_reg)
		, _exception_controller(_nvic, _shpr2_reg, _shpr3_reg, _icsr_reg, other._exception_controller)
		, _control_signals(other._control_signals)
		, _memory(_exception_controller, generate_system_control_register_map())
		, _cpu(_memory, _exception_controller, _control_signals, other._cpu)
		, _systick(_exception_controller, other._systick)
		, _io_reg_callback(other._io_reg_callback)
		, _generic_io_reg(_io_reg_callback)
		, _cpuid_reg(other._cpuid_reg)
		, _aircr(_exception_controller, _control_signals.reset)
		, _vtable_offset_reg(other._vtable_offset_reg)
		, _icsr_reg(other._icsr_reg)
		, _usart_controller(_exception_controller, exception::EXTI_00)
		, _previously_used_entrypoint(other._previously_used_entrypoint) {}

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

	const exception_controller& exceptions() const {
		return _exception_controller;
	}

	exception_controller& exceptions() {
		return _exception_controller;
	}

	usart_controller& get_usart_controller() {
		return _usart_controller;
	}

	void set_io_callback(generic_io_reg::callback_t callback) {
		_io_reg_callback = std::move(callback);
	}

	void reset(uint32_t program_entry_point) {
		_previously_used_entrypoint = program_entry_point;
		_usart_controller.reset();
		_systick.reset();
		_shpr2_reg.reset();
		_shpr3_reg.reset();
		_nvic.reset();
		_exception_controller.reset();
		_cpu.reset(program_entry_point);
	}

	void reset() {
		reset(_previously_used_entrypoint);
	}

	step_result step();
};

} // namespace micromachine::system
