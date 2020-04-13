/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "include/tinyprintf.h"
#include "include/control_registers.h"

static
void io_call(uint8_t op, uint8_t d0, uint8_t d1, uint8_t d2) {
	IO_REG = ((op << 24) | (d2 << 16) | (d2 << 8) | (d0 << 0));
}

void _putc(char c) {
	io_call(0, c, 0, 0);
}

static
void _tinyprintf_putc(void* ptr, char c) {
	_putc(c);
}

void _init_io() {
	init_printf(NULL, _tinyprintf_putc);
}

