#ifndef MICROMACHINE_IPSR_REGSITER_HPP
#define MICROMACHINE_IPSR_REGSITER_HPP

#include "types.hpp"
#include "xpsr_reg.hpp"
#include "exception_type.hpp"

struct ipsr_reg : public xpsr_reg {

	using xpsr_reg::xpsr_reg;
	// IPSR bits are XPSR's bit 0 to 5
	using ipsr_bits = bits<0, 6>;

	void set_exception_number(exception_number number) {
		ipsr_bits::of(_xpsr) = ipsr_bits::of(number.int_value());
	}

	exception_number exception_num() const {
		return exception_number::from_uint(ipsr_bits::of(_xpsr));
	}

private:

	using ipsr_slice = ipsr_bits::integer_slice<uint32_t>;
	using ipsr_const_slice = ipsr_bits::const_integer_slice<uint32_t>;


};


#endif //MICROMACHINE_IPSR_REGSITER_HPP
