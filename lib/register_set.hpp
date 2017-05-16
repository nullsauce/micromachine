//
// Created by fla on 17-5-9.
//

#ifndef MICROMACHINE_REGISTER_TYPES_HPP
#define MICROMACHINE_REGISTER_TYPES_HPP


#include "register.hpp"


struct register_set {

	word get(reg_idx index) const {

		const size_t i = index & 15;

		word val = _registers[i];

		if(i > 7) {
			if(i < 13) {
				// high reg
			} else if(i == 13) {
				val = sp_after_read(val);
			} else if(i == 15) {
				val = pc_after_read(val);
			}
		}

		return val;
	}

	word getSP() const {
		return get(13);
	}
	void setSP(word val) {
		set(13, val);
	}

	word getPC() const {
		return get(15);
	}

	void setPC(word val) {
		set(15, val);
	}

	void set(reg_idx i, word val) {

		precond(i <= 15, "register index too large");

		//fprintf(stderr, "write %02d : %s\n", i, val.to_string().c_str());

		if(i > 7) {
			if(i < 13) {
				// high reg
			} else if(i == 13) {
				val = sp_before_write(val);
			} else if(i == 15) {
				pc_is_dirty = true;
				val = pc_before_write(val);
			}
		}

		_registers[i] = val;
	}

	void reset_pc_dirty_status() {
		pc_is_dirty = false;
	}

	bool branch_occured() const {
		return pc_is_dirty;
	}

	void print() {
		for(size_t i = 0; i < 16; i++) {
			fprintf(stderr, "[%02zu] %08X\n", i, (uint32_t)get(i));
		}
	}


private:

	static word sp_before_write(word word) {
		// these two bits should always be zero, or UNPREDICTABLE
		if(word.any_bits_set(0, 2)) {
			// unpredicatable
		}
	}

	static word sp_after_read(word word) {
		return word & binops::make_mask<2, 30>();
	}

	static word pc_before_write(word word) {

		// if EPSR.T == 0, a UsageFault('Invalid State')
		// is taken on the next instruction
		if(word.bit(0) == 0) {
			// usage fault
		}

		return word & binops::make_mask<1,31>();
	}

	static word pc_after_read(word word) {
		return word & binops::make_mask<1,31>();
	}

	reg _registers[16];

	bool pc_is_dirty = false;
};


#endif //MICROMACHINE_REGISTER_TYPES_HPP
