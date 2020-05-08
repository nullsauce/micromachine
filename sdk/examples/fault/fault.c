/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include <stdint.h>
#include <stddef.h>

#include "alib.h"

int main() {
	volatile uint32_t* bad = (uint32_t*)a_lib_value;
	while(1) *bad++;
}


