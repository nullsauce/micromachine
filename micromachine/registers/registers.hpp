//
// Created by fla on 17-5-9.
//

#ifndef MICROMACHINE_REGISTER_TYPES_HPP
#define MICROMACHINE_REGISTER_TYPES_HPP




#include "registers/exec_mode_reg.hpp"
#include "registers/control_reg.hpp"
#include "registers/standard_reg.hpp"
#include "registers/sp_reg.hpp"
#include "registers/pc_reg.hpp"
#include "registers/apsr_reg.hpp"
#include "registers/ipsr_reg.hpp"
#include "registers/epsr_reg.hpp"
#include "registers/primask_reg.hpp"
#include "exception_return_handler.hpp"

//#define USE_INDIRECT_REG_ACCESS
class registers {
private:
	std::array<standard_reg, 13> _gen_pupose_registers;
	control_reg _control_register;
	exec_mode_reg _exec_mode_register;
	uint32_t _xpsr_register;
	apsr_reg _app_status_register;
	ipsr_reg _interrupt_status_register;
	epsr_reg _execution_status_register;
	primask_reg _primask_register;
	sp_reg _sp;
	standard_reg _lr;
	pc_reg _pc;

public:
	registers(exception_return_handler& exception_return_handler)
		: _xpsr_register(0)
		, _app_status_register(_xpsr_register)
		, _interrupt_status_register(_xpsr_register)
		, _execution_status_register(_xpsr_register)
		, _sp(_exec_mode_register, _control_register)
		, _pc(_exec_mode_register, _execution_status_register, exception_return_handler)
	{}

	registers(exception_return_handler& exception_return_handler, const registers& existing_state)
		: _gen_pupose_registers(existing_state._gen_pupose_registers)
		, _control_register(existing_state._control_register)
		, _exec_mode_register(existing_state._exec_mode_register)
		, _xpsr_register(existing_state._xpsr_register)
		, _app_status_register(_xpsr_register)
		, _interrupt_status_register(_xpsr_register)
		, _execution_status_register(_xpsr_register)
		, _primask_register(existing_state._primask_register)
		, _sp(_exec_mode_register, _control_register, existing_state._sp)
		, _lr(existing_state._lr)
		, _pc(_exec_mode_register, _execution_status_register, exception_return_handler, existing_state._pc)
	{
		int k = 0;
	}

	// Number general purpose registers. Here 'general purpose' means
	// the low registers R0 to R7 AND the high registers R8 to R12
	static constexpr size_t NUM_GP_REGS = 13;
	static constexpr size_t NUM_REGS = NUM_GP_REGS + 3;
	static constexpr reg_idx R0 = 0;
	static constexpr reg_idx R1 = 1;
	static constexpr reg_idx R2 = 2;
	static constexpr reg_idx R3 = 3;
	static constexpr reg_idx R4 = 4;
	static constexpr reg_idx R5 = 5;
	static constexpr reg_idx R6 = 6;
	static constexpr reg_idx R7 = 7;
	static constexpr reg_idx R8 = 8;
	static constexpr reg_idx R9 = 9;
	static constexpr reg_idx R10 = 10;
	static constexpr reg_idx R11 = 11;
	static constexpr reg_idx R12 = 12;
	static constexpr reg_idx SP = 13;
	static constexpr reg_idx LR = 14;
	static constexpr reg_idx PC = 15;

	void reset() {
		for (reg_idx i = 0; i < NUM_REGS; i++) {
			set(i, 0);
		}
	}

	uint32_t get(reg_idx i) const {
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
	}

	void set(reg_idx i, uint32_t val) {
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

	void branch_interworking(uint32_t address) {
		_pc.branch_interworking(address);
	}

	void branch_link_interworking(uint32_t address) {
		_pc.branch_link_interworking(address);
	}

	void branch_alu(uint32_t address) {
		_pc.branch(address);
	}

	uint32_t get_sp() const {
		return get(SP);
	}

	void set_sp(uint32_t val) {
		set(SP, val);
	}

	uint32_t get_lr() const {
		return get(LR);
	}

	void set_lr(uint32_t val) {
		set(LR, val);
	}

	uint32_t get_pc() const {
		return get(PC);
	}

	void set_pc(uint32_t val) {
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

	primask_reg& primask_register() {
		return _primask_register;
	}

	const primask_reg& primask_register() const {
		return _primask_register;
	}

	uint32_t& xpsr_register() {
		return _xpsr_register;
	}

	const uint32_t& xpsr_register() const {
		return _xpsr_register;
	}

};


#endif //MICROMACHINE_REGISTER_TYPES_HPP
