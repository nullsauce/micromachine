#pragma once

#include "../../exception/exception.hpp"
#include "../../types/bits.hpp"
#include "../../types/types.hpp"
#include "xpsr_reg.hpp"

namespace micromachine::system {

struct ipsr_reg : public xpsr_reg {

	using xpsr_reg::xpsr_reg;

	using ipsr_bits = bits<0, 6>;

	void set_exception_number(exception exception) {
		ipsr_bits::of(_xpsr) = ipsr_bits::of((uint32_t)exception);
	}

	micromachine::system::exception exception() const {
		return exception::from_number(ipsr_bits::of(_xpsr));
	}

	void reset() {
		ipsr_bits::of(_xpsr) = 0U;
	}

private:
	using ipsr_slice = ipsr_bits::integer_slice<uint32_t>;
	using ipsr_const_slice = ipsr_bits::const_integer_slice<uint32_t>;
};

} // namespace micromachine::system
