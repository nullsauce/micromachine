//
// Created by fla on 17-5-9.
//

#ifndef MICROMACHINE_REGISTER_TYPES_HPP
#define MICROMACHINE_REGISTER_TYPES_HPP



#include "exception_vector.hpp"
#include "registers/exec_mode_reg.hpp"
#include "registers/control_reg.hpp"
#include "registers/standard_reg.hpp"
#include "registers/sp_reg.hpp"
#include "registers/pc_reg.hpp"
#include "registers/apsr_reg.hpp"
#include "registers/ipsr_reg.hpp"
#include "registers/epsr_reg.hpp"
#include "exception_return_handler.hpp"
//#define USE_INDIRECT_REG_ACCESS
struct registers {

	registers(exception_return_handler& exception_return_handler)
		: _xpsr_register(0)
		, _app_status_register(_xpsr_register)
		, _interrupt_status_register(_xpsr_register)
		, _execution_status_register(_xpsr_register)
		, _sp(_exec_mode_register, _control_register)
		, _pc(_exec_mode_register, _execution_status_register, exception_return_handler) {

	}

	static constexpr size_t NUM_GP_REGS = 13; // general purpose registers
	static constexpr size_t NUM_REGS = NUM_GP_REGS + 3;
	static constexpr reg_idx SP = 13;
	static constexpr reg_idx LR = 14;
	static constexpr reg_idx PC = 15;

	void reset() {
		for(reg_idx i = 0; i < NUM_REGS; i++) {
			set(i, 0);
		}
		_execution_status_register.set_thumb_bit(1);
	}

	word get(reg_idx i) const {
	#ifdef USE_INDIRECT_REG_ACCESS
		return *_registers[i];
	#else
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
	#endif
	}

	void set(reg_idx i, word val) {
	#ifdef USE_INDIRECT_REG_ACCESS
		(*_registers[i]) = val;
	#else
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
	#endif
	}

	void branch_interworking(word address) {
		_pc.branch_interworking(address);
	}

	void branch_link_interworking(word address) {
		_pc.branch_link_interworking(address);
	}

	void branch_alu(word address) {
		_pc.branch(address);
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

	word& primask_register() {
		return _primask_register;
	}

	const word& primask_register() const {
		return _primask_register;
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
	word 			_primask_register;
	sp_reg 			_sp;
	standard_reg 	_lr;
	pc_reg 			_pc;
	const std::array<ireg*, 16> _registers = {
		&_gen_pupose_registers[0],
		&_gen_pupose_registers[1],
		&_gen_pupose_registers[2],
		&_gen_pupose_registers[3],
		&_gen_pupose_registers[4],
		&_gen_pupose_registers[5],
		&_gen_pupose_registers[6],
		&_gen_pupose_registers[7],
		&_gen_pupose_registers[8],
		&_gen_pupose_registers[9],
		&_gen_pupose_registers[10],
		&_gen_pupose_registers[11],
		&_gen_pupose_registers[12],
		&_sp,
		&_lr,
		&_pc
	};


};


#endif //MICROMACHINE_REGISTER_TYPES_HPP
