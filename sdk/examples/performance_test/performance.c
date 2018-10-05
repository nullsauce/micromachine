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

extern char _heap_start;



void io_call(uint8_t op, uint8_t d0, uint8_t d1, uint8_t d2) {
	IO_REG = ((op << 24) | (d2 << 16) | (d2 << 8) | (d0 << 0));
}

void putc(char c) {
	io_call(0, c, 0, 0);
}

void printf_putc(void* ptr, char c) {
	putc(c);
}

void puts(char* str) {
	while(*str) {
		io_call(0, *(str++), 0, 0);
	}
}

const char* hex = "0123456789abcdef";

// Initialize SysTick with busy wait running at bus clock.
void init_systick(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = 1000000;  			// reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
                                        // enable SysTick with core clock
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE + NVIC_ST_CTRL_CLK_SRC;
}


//#define ISR __attribute__((weak, interrupt("IRQ")))

void entry();

INTERRUPT_USER_OVERRIDE(_isr_hardfault)
void _user_isr_hardfault(void) {
	// 1) peek the return addres from the stack into r0
	register uint32_t* stack asm("sp");
	breakpoint(0x42);
};

INTERRUPT_USER_OVERRIDE(_isr_systick)
void _user_isr_systick() {
	volatile uint32_t* heap = (uint32_t*)&_heap_start;
	heap[16]++;
	uint8_t byte = heap[16] & 0xff;
	char a = hex[(byte >> 0) & 0xf];
	printf("Hello World %08x\n", heap[16]);
};

INTERRUPT_USER_OVERRIDE(_isr_reset)
void _user_isr_reset() {
	// zero bss
	extern uint32_t* __bss_start__;
	extern uint32_t* __bss_end__;
	for (uint8_t* dst = (uint8_t*)__bss_start__; dst < (uint8_t*)__bss_end__; dst++) {
		*dst = 0;
	}
	entry();
}


static uint32_t z1 = 0x2f312e94;
static uint32_t z2 = 0xfebf9a50;
static uint32_t z3 = 0xbdd3a43c;
static uint32_t z4 = 0x41864de9;

uint32_t rand() {
	uint32_t b;
	b  = ((z1 << 6) ^ z1) >> 13;
	z1 = ((z1 & 4294967294U) << 18) ^ b;
	b  = ((z2 << 2) ^ z2) >> 27;
	z2 = ((z2 & 4294967288U) << 2) ^ b;
	b  = ((z3 << 13) ^ z3) >> 21;
	z3 = ((z3 & 4294967280U) << 7) ^ b;
	b  = ((z4 << 3) ^ z4) >> 12;
	z4 = ((z4 & 4294967168U) << 13) ^ b;
	return (z1 ^ z2 ^ z3 ^ z4);
}

int fib(int n){
	if ( n == 0 ) {
		return 0;
	} else if ( n == 1 ) {
		return 1;
	} else {
		return (fib(n-1) + fib(n-2));
	}
}

void entry() {
	init_printf(NULL, printf_putc);
	init_systick();
	volatile uint32_t* heap = (uint32_t*)&_heap_start;
	//for(uint32_t i = 0; i < 1024*100; i++) {
	while(1) {
		volatile uint32_t* target = &heap[rand() % 16];
		int f = fib(rand() % 16);
		(*target) = f; //;
	}
	breakpoint(0);
}


