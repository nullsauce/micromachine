/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_EXCEPTION_RETURN_HANDLER_HPP
#define MICROMACHINE_EMU_EXCEPTION_RETURN_HANDLER_HPP

#include <cstdint>

struct exception_return_handler {
	virtual void exception_return(uint32_t ret_address) = 0;
};


#endif //MICROMACHINE_EMU_EXCEPTION_RETURN_HANDLER_HPP
