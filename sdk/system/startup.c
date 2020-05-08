/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "include/startup.h"
#include "include/system.h"
#include "include/instructions.h"

#include <stdint.h>

extern int main(void);

static
void _init_memory() {

	for(uint32_t i = 0; i < _section_bss_size; i++) {
		((uint8_t*)_section_bss_start)[i] = 0;
	}

	const uint8_t* init_data_ptr = (uint8_t*)_section_init_data_start;
	const uint8_t* init_data_end = (uint8_t*)_section_init_data_end;
	uint8_t* data_ptr = (uint8_t*)_section_data_start;
	// for some reason, a standard loop with array access similar to
	// the one used in the bss zeroing wont work as expected here and
	// will run even when not expected to do so.
	for(; init_data_ptr < init_data_end; init_data_ptr++, data_ptr++) {
		*data_ptr = *init_data_ptr;
	}
}

void _init() {
	_init_memory();
}

void _startup() {

	// init memory and io subsystems
	_init();

	// call main
	main();

	// terminate
	breakpoint(0xff);
}
