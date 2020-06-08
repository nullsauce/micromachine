
#include <system.h>
#include <stdio.h>

uint32_t test_variable_1;

// should be equivalent as above
static uint32_t test_variable_2;

void main() {
	static uint32_t test_variable_3;
	printf("%08x %i\n", test_variable_1, _section_bss_contains(&test_variable_1));
	printf("%08x %i\n", test_variable_2, _section_bss_contains(&test_variable_2));
	printf("%08x %i\n", test_variable_3, _section_bss_contains(&test_variable_3));
}
