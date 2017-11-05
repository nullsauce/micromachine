//
// Created by fla on 17-5-9.
//

#ifndef MICROMACHINE_REGISTER_TYPES_HPP
#define MICROMACHINE_REGISTER_TYPES_HPP


#include "register.hpp"
#include "exec_mode_reg.hpp"
#include "exception_vector.hpp"
#include "apsr_reg.hpp"
#include "ipsr_reg.hpp"
#include "epsr_reg.hpp"


struct registers {

	registers(exception_vector::bitref_t& hardfault_signal)
		: _xpsr_register(0)
		, _app_status_register(_xpsr_register)
		, _interrupt_status_register(_xpsr_register)
		, _execution_status_register(_xpsr_register)
		, _sp(_exec_mode_register, _control_register)
		, _pc(_exec_mode_register, hardfault_signal) {

	}

	static constexpr size_t NUM_GP_REGS = 13; // general purpose registers
	static constexpr size_t NUM_REGS = NUM_GP_REGS + 3;
	static constexpr reg_idx SP = 13;
	static constexpr reg_idx LR = 14;
	static constexpr reg_idx PC = 15;

	void reset() {
		for(reg_idx i = 0; i < NUM_GP_REGS; i++) {
			set(i, 0);
		}
	}

	word get(reg_idx i) const {

		precond(i < NUM_REGS, "register index too large %zu", i);

		if(i < NUM_GP_REGS) {
			return _gen_pupose_registers[i];
		} else if(i == SP) {
			return _sp;
		} else if(i == LR) {
			return _lr;
		} else if(i == PC) {
			return _pc;
		} else {
			precond_fail("invalid register index %lu", i);
		}

		return 0;
	}

	void set(reg_idx i, word val) {

		precond(i < NUM_REGS, "register index too large");

		if(i < NUM_GP_REGS) {
			_gen_pupose_registers[i] = val;
		} else if(i == SP) {
			_sp = val;
		} else if(i == LR) {
			_lr = val;
		} else if(i == PC) {
			_pc = val;
		} else {
			precond_fail("invalid register index %lu", i);
		}

	}

	void branch(word address) {
		_pc.branch(address);
	}

	void branch_thumb(word address) {
		_pc.branch(address | 1);
	}

	word get_sp() const {
		return get(SP);
	}

	void set_sp(word val) {
		set(SP, val);
	}

	word get_lr() const {
		return get(LR);
	}

	void set_lr(word val) {
		set(LR, val);
	}

	word get_pc() const {
		return get(PC);
	}

	void set_pc(word val) {
		set(PC, val);
	}

	void reset_pc_dirty_status() {
		_pc._dirty_status = false;
	}

	bool branch_occured() const {
		return _pc._dirty_status;
	}

	void print() {
		fprintf(stderr, "[SP] %08x\n", (uint32_t)get(SP));
		fprintf(stderr, "[LR] %08x\n", (uint32_t)get(LR));
		fprintf(stderr, "[PC] %08x\n", (uint32_t)get(PC));

		for(size_t i = 0; i < NUM_GP_REGS; i++) {
			fprintf(stderr, "[%02zu] %08X\n", i, (uint32_t)get(i));
		}
	}

	const control_reg& control_register() const {
		return _control_register;
	}

	control_reg& control_register() {
		return _control_register;
	}

	// to be used only for system instructions such as MSR
	sp_reg& sp_register() {
		return _sp;
	}

	exec_mode_reg& exec_mode_register() {
		return _exec_mode_register;
	}

	const exec_mode_reg& exec_mode_register() const {
		return _exec_mode_register;
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

	word& xpsr_register() {
		return _xpsr_register;
	}

	const word& xpsr_register() const {
		return _xpsr_register;
	}

private:

	standard_reg 	_gen_pupose_registers[13];
	control_reg 	_control_register;
	exec_mode_reg 	_exec_mode_register;
	word 			_xpsr_register;
	apsr_reg 		_app_status_register;
	ipsr_reg 		_interrupt_status_register;
	epsr_reg		_execution_status_register;
	sp_reg 			_sp;
	standard_reg 	_lr;
	pc_reg 			_pc;


};


#endif //MICROMACHINE_REGISTER_TYPES_HPP
