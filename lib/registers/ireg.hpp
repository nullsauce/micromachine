/* Copyright (C) The Micromachine project - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of The Micromachine project.

*/

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
