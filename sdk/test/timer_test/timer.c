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
#include <io.h>

void _isr_systick() {
	static uint32_t counter = 0;
	printf("Systick interrupt %d\n", counter++);
	if(counter == 10) {
		breakpoint(0xff);
	}
};

void main() {
	NVIC_ST_CTRL_R = 0;			// disable SysTick during setup
	NVIC_ST_RELOAD_R = 100000;	// reload value
	NVIC_ST_CURRENT_R = 0;		// any write to current clears it
	// enable SysTick with core clock
	NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE + NVIC_ST_CTRL_CLK_SRC;
	while(1) {}
}


