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
		, _exception_return_handler(exception_return_handler)
	{}

	pc_reg(const exec_mode_reg& exec_mode_reg,
			epsr_reg& epsr_reg,
			exception_return_handler& exception_return_handler,
			const pc_reg& existing_state)
		: standard_reg(existing_state)
		, _exec_mode_reg(exec_mode_reg)
		, _epsr_reg(epsr_reg)
		, _exception_return_handler(exception_return_handler)
	{}

	void branch(uint32_t address) {
		set(address & 0xFFFFFFFE);
	}

	void branch_interworking(uint32_t address) {
		if(_exec_mode_reg.is_handler_mode() && 0b1111 == bits<28,4>::of(address)) {
			uint32_t return_address = bits<0,28>::of(address);
			_exception_return_handler.exception_return(return_address);
		} else {

			if(!bits<0>::of(address)) fprintf(stderr, "HARDFAULT NEXT (branch_interworking)\n");
			// trigger a hard fault on next instruction if Thumb bit is not set
			_epsr_reg.set_thumb_bit(bits<0>::of(address));

			/*
			if(!bits<0>::of(address)) {
				// Thumb bit not set, triggers a fault
				_hardfault_signal = true;
				fprintf(stderr, "PC: Thumb bit not set\n");
			}*/

			// Inter-working branch, thumb bit is always cleared
			branch(address);
		}

	}

	void branch_link_interworking(uint32_t address) {
		// if the thumb bit is not set, a hard fault must be taken on the next instruction
		// copy the last bit of jump_addr to allow this to happen
		_epsr_reg.set_thumb_bit(bits<0>::of(address));

		// Inter-working branch, thumb bit is always cleared
		bits<0>::of(address) = false;
		branch(address);
	}

private:

	static constexpr uint32_t MASK = binops::make_mask<uint32_t>(1, 31);

	void set(uint32_t word) override {
		_word = word;
		_dirty_status = true;
	}

	uint32_t get() const override {
		uint32_t val = _word & MASK;
		return val;
	}

	const exec_mode_reg& 			_exec_mode_reg;
	epsr_reg& 						_epsr_reg;
	exception_return_handler&		_exception_return_handler;
};


#endif //MICROMACHINE_PC_REG_HPP
