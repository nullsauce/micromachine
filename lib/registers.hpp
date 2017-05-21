//
// Created by fla on 17-5-9.
//

#ifndef MICROMACHINE_REGISTER_TYPES_HPP
#define MICROMACHINE_REGISTER_TYPES_HPP


#include "register.hpp"
#include "exec_mode.hpp"
#include "exception_vector.hpp"


struct registers {

	registers(exec_mode& exec_mode, exception_vector::bitref_t& hardfault_signal)
		: _sp(exec_mode, _control_register)
		, _pc(exec_mode, hardfault_signal) {

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
			return _gp_registers[i];
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
			_gp_registers[i] = val;
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


private:

	standard_reg 	_gp_registers[13];
	control_reg 	_control_register;
	sp_reg 			_sp;
	standard_reg 	_lr;
	pc_reg 			_pc;

};


#endif //MICROMACHINE_REGISTER_TYPES_HPP
