
#include <stdint.h>
#include <stddef.h>
#include <system.h>
#include <stdio.h>

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


