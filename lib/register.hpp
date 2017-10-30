//
// Created by fla on 17-5-9.
//

#ifndef MICROMACHINE_REGISTER_HPP
#define MICROMACHINE_REGISTER_HPP

#include "types.hpp"
#include "exec_mode.hpp"
#include "exception_vector.hpp"


class ireg {

public:

	ireg& operator = (word word) {
		set(word);
		return *this;
	}

	operator word() const {
		return get();
	}

private:

	virtual void set(word word) = 0;
	virtual const word get() const = 0;

};

class standard_reg : public ireg {
public:

	using ireg::operator=;
	standard_reg() : _word(0) {}

private:

	void set(word word) override {
		_word = word;
	}

	const word get() const override {
		return _word;
	}

protected:
	word _word;
};


class control_reg : public standard_reg {
public:

	using standard_reg::standard_reg;
	using standard_reg::operator=;

	bool n_priv() const {
		return _word.bit(0);
	}

	bool sp_sel() const {
		return _word.bit(1);
	}

	void set_sp_sel(bool sel) {
		_word.write_bit(1, sel);
	}

};

class sp_reg : public ireg {
public:

	sp_reg(const exec_mode& exec_mode, const control_reg& control_reg)
		: _exec_mode(exec_mode)
		, _ctl_reg(control_reg) {

	}

	using ireg::operator=;

private:

	static const uint32_t MASK = binops::make_mask<2, 30>();

	virtual void set(word word)  {
		// these two bits should always be zero, or UNPREDICTABLE
		if(word & MASK) {
			// unpredicatable
		}
		banked() = word;
	}

	virtual const word get() const  {
		return banked() & MASK;
	}


	word& banked() {
		switch (_exec_mode) {
			case exec_mode::thread:
				return _sps[_ctl_reg.sp_sel()];
			case exec_mode::handler:
				return _sps[0];
			default: precond_fail("invalid execution mode")
		}
	}

	const word& banked() const {
		switch (_exec_mode) {
			case exec_mode::thread:
				return _sps[_ctl_reg.sp_sel()];
			case exec_mode::handler:
				return _sps[0];
			default: precond_fail("invalid execution mode")
		}
	}

	// stack pointers
	// 0: SP Main
	// 1: SP Process
	word _sps[2];


	const exec_mode& _exec_mode;
	const control_reg& _ctl_reg;
};


class pc_reg : public standard_reg {
public:

	using standard_reg::standard_reg;
	using standard_reg::operator=;

	bool _dirty_status;

	pc_reg(const exec_mode& exec_mode, exception_vector::bitref_t& hardfault_signal)
		: _exec_mode(exec_mode)
		, _hardfault_signal(hardfault_signal) {

	}

	void branch(word address) {

		// if EPSR.T == 0, a UsageFault('Invalid State')
		// is taken on the next instruction

		if(exec_mode::handler == _exec_mode &&
			0b1111 == address.uint(28,4)) {
			// TODO ExceptionReturn
			precond_fail("ExceptionReturn unimplemented")
		}

		if(!address.bit(0)) {
			// Thumb bit not set, triggers a fault
			_hardfault_signal = true;
		}

		set(address);
	}

private:

	static const uint32_t MASK = binops::make_mask<1, 31>();

	virtual void set(word word)  {
		_word = word;
		_dirty_status = true;
	}

	virtual const word get() const  {
		word val = _word & MASK;
		return val;
	}

	const exec_mode& 			_exec_mode;
	exception_vector::bitref_t&	_hardfault_signal;
};




#endif //MICROMACHINE_REGISTER_HPP
