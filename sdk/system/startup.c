/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "include/startup.h"
#include "include/system.h"
#include "include/entrypoint.h"
#include "include/instructions.h"

#include <stdint.h>

void _startup() {

	// zero out the bss
	for (uint8_t* dst = (uint8_t*)_system_bss_start; dst < (uint8_t*)_system_bss_end; dst++) {
		*dst = 0;
	}

	// call main
	main();

	// terminate
	breakpoint(0xff);
}
