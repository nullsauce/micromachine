//
// Created by fla on 17-5-9.
//

#ifndef MICROMACHINE_REGISTER_HPP
#define MICROMACHINE_REGISTER_HPP

#include "types.hpp"

using reg_idx = size_t;

class reg {

public:

	reg() : _word(0) {

	}

	reg& operator = (word word) {
		set(word);
		return *this;
	}

	operator word() const {
		return get();
	}

	// explicit conversion to underlying type
	operator word::bits_t() const {
		return get();
	}

private:

	void set(word word)  {
		_word = word;
	}

	const word get() const  {
		return _word;
	}


	word _word;
};

#endif //MICROMACHINE_REGISTER_HPP
