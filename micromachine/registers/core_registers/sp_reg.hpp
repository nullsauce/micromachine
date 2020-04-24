#pragma once

#include "../../execution/execution_mode.hpp"
#include "../../helpers/binops.hpp"
#include "../../types/bits.hpp"
#include "../iregister.hpp"
#include "../standard_reg.hpp"
#include "registers/special_registers/control_reg.hpp"

namespace micromachine::system {

class sp_reg : public iregister {
private:
	// Banked stack pointers
	// 0: SP Main
	// 1: SP Process
	std::array<uint32_t, 2> _banked_stack_pointers;
	const execution_mode& _execution_mode;
	const control_reg& _control_reg;

public:
	sp_reg(const execution_mode& execution_mode, const control_reg& control_reg)
		: _banked_stack_pointers{{0, 0}}
		, _execution_mode(execution_mode)
		, _control_reg(control_reg) {}

	sp_reg(const execution_mode& execution_mode,
		   const control_reg& control_reg,
		   const sp_reg& existing_state)
		: _banked_stack_pointers(existing_state._banked_stack_pointers)
		, _execution_mode(execution_mode)
		, _control_reg(control_reg) {}

	using iregister::operator=;

	enum class StackType : size_t { Main = 0, Process = 1 };

	void set_specific_banked_sp(StackType type, uint32_t word) {
		_banked_stack_pointers[(size_t)type] = word;
	}

	uint32_t get_specific_banked_sp(StackType type) {
		return _banked_stack_pointers[(size_t)type];
	}

	void reset() override {
		set(0);
	}

private:
	static constexpr uint32_t VALUE_MASK = binops::make_mask<uint32_t>(2, 30);

	void set(uint32_t word) override {
		// these two bits should always be zero, or UNPREDICTABLE
		if(bits<0, 2>::of(word)) {
			// unpredictable
		}
		current_banked_value() = word;
	}

	uint32_t get() const override {
		return VALUE_MASK & current_banked_value();
	}

	uint32_t& current_banked_value() {
		if(_execution_mode.is_in_thread_mode()) {
			return _banked_stack_pointers[_control_reg.sp_sel()];
		} else {
			// handler mode
			return _banked_stack_pointers[0];
		}
	}

	const uint32_t& current_banked_value() const {
		if(_execution_mode.is_in_thread_mode()) {
			return _banked_stack_pointers[_control_reg.sp_sel()];
		} else {
			// handler mode
			return _banked_stack_pointers[0];
		}
	}
};

} // namespace micromachine::system
