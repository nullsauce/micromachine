
#include <interrupt_handlers.h>
#include <instructions.h>

#include <micromachine.h>

#include <system.h>
#include <stdio.h>

void _isr_systick() {
	static uint32_t counter = 0;
	printf("Systick interrupt %u\n", counter++);
	if(counter == 10) {
		breakpoint(0xff);
	}
};

void main() {

	NVIC_DisableIRQ(SysTick_IRQn); // disable SysTick during setup

	if(SysTick_Config(1000)) {
		printf("systick configuration failed\n");
	}

	NVIC_EnableIRQ(SysTick_IRQn);

	while(1) {}
}


