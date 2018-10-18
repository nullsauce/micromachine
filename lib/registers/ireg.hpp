#ifndef MICROMACHINE_IREG_HPP
#define MICROMACHINE_IREG_HPP

#include "types.hpp"

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

	virtual void set(word word) = 0;
	virtual word get() const = 0;


};

#endif //MICROMACHINE_IREG_HPP
