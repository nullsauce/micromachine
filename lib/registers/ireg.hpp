#ifndef MICROMACHINE_IREG_HPP
#define MICROMACHINE_IREG_HPP

#include "types.hpp"

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

#endif //MICROMACHINE_IREG_HPP
