#ifndef MICROMACHINE_IPSR_REGSITER_HPP
#define MICROMACHINE_IPSR_REGSITER_HPP

#include "types.hpp"
#include "xpsr_reg.hpp"
#include "exception_type.hpp"

struct ipsr_reg : public xpsr_reg {

	using xpsr_reg::xpsr_reg;

	void set_exception_number(exception_number number) {
		mutable_xpsr_bits() = (uint8_t)number;
	}

	exception_number exception_num() const {
		word bits = xpsr_bits();
		return exception_number::from_uint(bits);
	}

private:
	// IPSR bits are XPSR's bit 0 to 5
	using ipsr_bits = bitslice<0,6, word>;
	ipsr_bits mutable_xpsr_bits() {
		//TODO: bitslice::bits::of(_xpsr)
		return ipsr_bits(_xpsr);
	}

	const ipsr_bits xpsr_bits() const {
		//TODO: bitslice::bits::of(_xpsr)
		return ipsr_bits(_xpsr);
	}
};


#endif //MICROMACHINE_IPSR_REGSITER_HPP
