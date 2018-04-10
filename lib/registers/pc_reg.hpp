#ifndef MICROMACHINE_PC_REG_HPP
#define MICROMACHINE_PC_REG_HPP

#include "standard_reg.hpp"
#include "exec_mode_reg.hpp"
#include "epsr_reg.hpp"
#include "exception_return_handler.hpp"

class pc_reg : public standard_reg {
public:

	using standard_reg::standard_reg;
	using standard_reg::operator=;

	bool _dirty_status;

	pc_reg(const exec_mode_reg& exec_mode_reg,
			epsr_reg& epsr_reg,
			exception_return_handler& exception_return_handler)
		: _exec_mode_reg(exec_mode_reg)
		, _epsr_reg(epsr_reg)
		, _exception_return_handler(exception_return_handler) {

	}

	void branch(word address) {
		set(address & 0xFFFFFFFE);
	}

	void branch_interworking(word address) {
		if(_exec_mode_reg.is_handler_mode() &&
			0b1111 == address.uint(28,4)) {
			// TODO ExceptionReturn
			uint32_t return_address = address.uint(0, 28);
			_exception_return_handler.exception_return(return_address);
		} else {

			if(!address.bit(0)) fprintf(stderr, "HARDFAULT NEXT (branch_interworking)\n");
			// trigger a hard fault on next instruction if Thumb bit is not set
			_epsr_reg.set_thumb_bit(address.bit(0));

			/*
			if(!address.bit(0)) {
				// Thumb bit not set, triggers a fault
				_hardfault_signal = true;
				fprintf(stderr, "PC: Thumb bit not set\n");
			}*/

			// Inter-working branch, thumb bit is always cleared
			branch(address);
		}

	}

	void branch_link_interworking(word address) {
		if(!address.bit(0)) fprintf(stderr, "HARDFAULT NEXT (branch_interworking)\n");
		// trigger a hard fault on next instruction if Thumb bit is not set
		_epsr_reg.set_thumb_bit(address.bit(0));
		// Inter-working branch, thumb bit is always cleared
		branch(address);
	}

private:

	static const uint32_t MASK = binops::make_mask<1, 31>();

	void set(word word) override {
		_word = word;
		_dirty_status = true;
	}

	const word get() const override {
		word val = _word & MASK;
		return val;
	}

	const exec_mode_reg& 			_exec_mode_reg;
	epsr_reg& 						_epsr_reg;
	exception_return_handler&		_exception_return_handler;
};


#endif //MICROMACHINE_PC_REG_HPP
