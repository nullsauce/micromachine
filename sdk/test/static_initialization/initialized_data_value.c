/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/


#include <interrupt_handlers.h>
#include <instructions.h>
#include <systick.h>
#include <stdint.h>
#include <stddef.h>
#include <tinyprintf.h>
#include <entrypoint.h>
#include <random.h>
#include <system.h>

void io_call(uint8_t op, uint8_t d0, uint8_t d1, uint8_t d2) {
	IO_REG = ((op << 24) | (d2 << 16) | (d2 << 8) | (d0 << 0));
}

void printf_putc(void* ptr, char c) {
	io_call(0, c, 0, 0);
}


#define IS_IN_DATA(var) (((char*)&var) >= _system_data_start && ((char*)&var) < _system_data_end)


uint32_t test_variable_1 = 0x99c0feee;

// should be equivalent as above
static uint32_t test_variable_2 = 0xbeefc0de;

void main() {
	static uint32_t test_variable_3 = 0x42424242;
	init_printf(NULL, printf_putc);
	printf("%08x %i\n", test_variable_1, IS_IN_DATA(test_variable_1));
	printf("%08x %i\n", test_variable_2, IS_IN_DATA(test_variable_2));
	printf("%08x %i\n", test_variable_3, IS_IN_DATA(test_variable_3));
}
