

#include <system.h>
#include <stdio.h>

uint32_t test_variable_1 = 0x99c0feee;

// should be equivalent as above
static uint32_t test_variable_2 = 0xbeefc0de;

void main() {
	static uint32_t test_variable_3 = 0x42424242;
	printf("%08x %i\n", test_variable_1, _section_data_contains(&test_variable_1));
	printf("%08x %i\n", test_variable_2, _section_data_contains(&test_variable_2));
	printf("%08x %i\n", test_variable_3, _section_data_contains(&test_variable_3));
}
