/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/
#include <stdint.h>
#include <stddef.h>
extern char _heap_start;

#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))
#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value


// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_RELOAD_R = 10000;  			// reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
                                        // enable SysTick with core clock
  NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE + NVIC_ST_CTRL_CLK_SRC;
}

#define BREAKPOINT(code) __asm__ __volatile__ ("bkpt %0" : : "I" (code) )
#define SVC(code) __asm__ __volatile__ ("svc %0" : : "I" (code) )
#define ISR __attribute__((weak, interrupt("IRQ")))
void _isr_empty() {};
void ISR _isr_nmi() {};
void ISR _isr_hardfault() {
	// 1) peek the return addres from the stack into r0
	register uint32_t* stack asm("sp");
	BREAKPOINT(0x42);
};
void ISR _isr_svcall() {};
void ISR _isr_pendsv() {};
void ISR _isr_systick() {
	volatile uint32_t* heap = (uint32_t*)&_heap_start;
	heap[16]++;
};
void ISR _isr_external_interruput() {};

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
	SysTick_Init();
	volatile uint32_t* heap = (uint32_t*)&_heap_start;
	for(uint32_t i = 0; i < 1024*10; i++) {
		volatile uint32_t* target = &heap[rand() % 16];
		int f = fib(rand() % 16);
		(*target) = f; //;
	}
	BREAKPOINT(0);
}


void _isr_reset() {
	// zero bss
	extern uint32_t* __bss_start__;
	extern uint32_t* __bss_end__;
	for (uint8_t* dst = (uint8_t*)__bss_start__; dst < (uint8_t*)__bss_end__; dst++) {
		*dst = 0;
	}
	entry();
}
