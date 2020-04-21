/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include <stdint.h>
#include <stddef.h>
#include <tinyprintf.h>
#include <system.h>

#define CPUID 0xE000ED00

void main() {

	printf("address = %08x\n", CPUID);

	// Copy the 4 bytes of the register into a string
	// and prints it.

	char cpu_id_str[5] = {0};

	*((uint32_t*)cpu_id_str) = *((volatile uint32_t*)CPUID);

	cpu_id_str[4] = 0;

	printf("value = '%s'\n", cpu_id_str);
}


