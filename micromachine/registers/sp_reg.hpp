#ifndef MICROMACHINE_SP_REG_HPP
#define MICROMACHINE_SP_REG_HPP

#include "standard_reg.hpp"
#include "control_reg.hpp"
#include "exec_mode_reg.hpp"

class sp_reg : public ireg {
public:

	sp_reg(const exec_mode_reg& exec_mode_reg, const control_reg& control_reg)
		: _sps{{0, 0}}
		, _exec_mode_reg(exec_mode_reg)
		, _ctl_reg(control_reg)
	{}

	sp_reg(const exec_mode_reg& exec_mode_reg, const control_reg& control_reg, const sp_reg& existing_state)
		: _sps(existing_state._sps)
		, _exec_mode_reg(exec_mode_reg)
		, _ctl_reg(control_reg)
	{}

	using ireg::operator=;

	enum class StackType : size_t {
		Main = 0,
		Process = 1
	};

	void set_specific_banked_sp(StackType type, uint32_t word) {
		//fprintf(stderr, "SET (specific) SP=%08x\n", word);
		_sps[(size_t)type] = word;
	}

	uint32_t get_specific_banked_sp(StackType type) {
		return _sps[(size_t)type];
	}

	void reset() override {
		set(0);
	}

private:

	static const uint32_t MASK = binops::make_mask<uint32_t>(2, 30);

	void set(uint32_t word) override {
		// these two bits should always be zero, or UNPREDICTABLE
		//fprintf(stderr, "SET SP=%08x\n", word);
		if(word & MASK) {
			// unpredictable
		}
		banked() = word;
	}

	uint32_t get() const override {
		return banked() & MASK;
	}

	uint32_t& banked() {
		if(_exec_mode_reg.is_thread_mode()) {
			return _sps[_ctl_reg.sp_sel()];
		} else  {
			// handler mode
			return _sps[0];
		}
	}

	const uint32_t& banked() const {
		if(_exec_mode_reg.is_thread_mode()) {
			return _sps[_ctl_reg.sp_sel()];
		} else {
			// handler mode
			return _sps[0];
		}
	}

	// stack pointers
	// 0: SP Main
	// 1: SP Process
	std::array<uint32_t, 2> _sps;

	const exec_mode_reg& _exec_mode_reg;
	const control_reg& _ctl_reg;
};


#endif //MICROMACHINE_SP_REG_HPP
