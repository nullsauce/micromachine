//
// Created by fla on 17-5-9.
//

#pragma once

#include "control_reg.hpp"
#include "exception_return_handler.hpp"
#include "pc_reg.hpp"
#include "registers/pc_reg.hpp"
#include "registers/registers.hpp"
#include "registers/sp_reg.hpp"
#include "registers/standard_reg.hpp"
#include "sp_reg.hpp"
#include "standard_reg.hpp"

namespace micromachine::system {

class core_registers {
private:
	std::array<standard_reg, 13> _general_purpose_registers;
	sp_reg _sp;
	standard_reg _lr;
	pc_reg _pc;

public:
	static constexpr size_t NUM_LOW_GP_REGS = 8;
	static constexpr size_t NUM_HIGH_GP_REGS = 5;
	static constexpr size_t NUM_GP_REGS = NUM_LOW_GP_REGS + NUM_HIGH_GP_REGS;
	static constexpr size_t NUM_CORE_REGS = NUM_GP_REGS + 3;


	core_registers(const execution_mode& execution_mode,
				   const control_reg& control_reg,
				   exception_return_handler& exception_return_handler)
		: _sp(execution_mode, control_reg)
		, _pc() {}

	core_registers(const execution_mode& execution_mode,
				   const control_reg& control_reg,
				   exception_return_handler& exception_return_handler,
				   const core_registers& existing_state)
		: _general_purpose_registers(existing_state._general_purpose_registers)
		, _sp(execution_mode, control_reg, existing_state._sp)
		, _lr(existing_state._lr)
		, _pc(existing_state._pc) {}

	void reset() {
		for (reg_idx i = 0; i < NUM_CORE_REGS; i++) {
			set(i, 0);
		}
	}

	uint32_t get(reg_idx i) const {
		precond(i < NUM_CORE_REGS, "register index too large %zu", i);
		if (i < NUM_GP_REGS) {
			return _general_purpose_registers[i];
		} else if (i == registers::SP) {
			return _sp;
		} else if (i == registers::LR) {
			return _lr;
		} else if (i == registers::PC) {
			return _pc;
		} else {
			precond_fail("invalid register index %lu", i);
		}
	}

	void set(reg_idx i, uint32_t val) {
		precond(i < NUM_CORE_REGS, "register index too large");
		if (i < NUM_GP_REGS) {
			_general_purpose_registers[i] = val;
		} else if (i == registers::SP) {
			_sp = val;
		} else if (i == registers::LR) {
			_lr = val;
		} else if (i == registers::PC) {
			_pc = val;
		} else {
			precond_fail("invalid register index %lu", i);
		}
	}

	void branch_alu(uint32_t address) {
		_pc.branch(address);
	}

	//TODO Below replace set(REG, val) and get(REG) by a direct call the appropriate reg (ie: _sp = val)
	uint32_t get_sp() const {
		return get(registers::SP);
	}

	void set_sp(uint32_t val) {
		set(registers::SP, val);
	}

	uint32_t get_lr() const {
		return get(registers::LR);
	}

	void set_lr(uint32_t val) {
		set(registers::LR, val);
	}

	uint32_t get_pc() const {
		return get(registers::PC);
	}

	void set_pc(uint32_t val) {
		set(registers::PC, val);
	}

	void reset_pc_dirty_status() {
		_pc.clear_dirty_status();
	}

	bool branch_occured() const {
		return _pc.dirty_status();
	}

	// to be used only for system instructions such as MSR
	sp_reg& sp_register() {
		return _sp;
	}

	pc_reg& pc_register() {
		return _pc;
	}
};

} // namespace micromachine::system
