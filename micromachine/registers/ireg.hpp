#ifndef MICROMACHINE_IREG_HPP
#define MICROMACHINE_IREG_HPP

#include "types.hpp"

namespace micromachine::system {

class ireg {
public:
	ireg& operator=(uint32_t word) {
		set(word);
		return *this;
	}

	operator uint32_t() const {
		return get();
	}

	virtual void reset() = 0;

private:
	virtual void set(uint32_t word) = 0;
	virtual uint32_t get() const = 0;

};

} // namespace micromachine::system

#endif //MICROMACHINE_IREG_HPP
