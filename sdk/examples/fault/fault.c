
#include <stdint.h>
#include <stddef.h>

#include "alib.h"

int main() {
	volatile uint32_t* bad = (uint32_t*)a_lib_value;
	while(1) (*bad)++;
}


