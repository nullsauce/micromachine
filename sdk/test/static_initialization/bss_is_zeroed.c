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
#include <io.h>
#include <startup.h>

uint32_t test_variable_1;

// should be equivalent as above
static uint32_t test_variable_2;

void main() {
	static uint32_t test_variable_3;
	printf("%08x %i\n", test_variable_1, _section_bss_contains(&test_variable_1));
	printf("%08x %i\n", test_variable_2, _section_bss_contains(&test_variable_2));
	printf("%08x %i\n", test_variable_3, _section_bss_contains(&test_variable_3));
}
