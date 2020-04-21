#ifndef MICROMACHINE_IPSR_REGSITER_HPP
#define MICROMACHINE_IPSR_REGSITER_HPP

#include "types.hpp"
#include "xpsr_reg.hpp"
#include "exception_defs.hpp"

struct ipsr_reg : public xpsr_reg {

	using xpsr_reg::xpsr_reg;

	using ipsr_bits = bits<0, 6>;

	void set_exception_number(exception::Type number) {
		ipsr_bits::of(_xpsr) = ipsr_bits::of(number);
	}

	exception::Type exception_num() const {
		return static_cast<exception::Type>(ipsr_bits::of(_xpsr).extract());
	}

	void reset() {
		ipsr_bits::of(_xpsr) = 0U;
	}

private:

	using ipsr_slice = ipsr_bits::integer_slice<uint32_t>;
	using ipsr_const_slice = ipsr_bits::const_integer_slice<uint32_t>;


};


#endif //MICROMACHINE_IPSR_REGSITER_HPP
