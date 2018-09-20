/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/
#include <stdint.h>
#include <stddef.h>
extern char _heap_start;

#define SVC(code) __asm__ __volatile__ ("svc %0" : : "I" (code) )
#define ISR __attribute__((weak, interrupt("IRQ")))
void _isr_empty() {};
void ISR _isr_nmi() {};
void ISR _isr_hardfault() {};
void ISR _isr_svcall() {};
void ISR _isr_pendsv() {};
void ISR _isr_systick() {};
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
	volatile uint32_t* heap = (uint32_t*)&_heap_start;
	heap[0] = 0x50414548;
	uint32_t i = 0;
	for(size_t i = 0; i < 1024*10; i++) {
		heap[rand() % 16] += (heap[fib(i % 16) % 16] | rand());
		heap[(heap[rand() % 16] % 4) * 4] -= fib(heap[i % 16] % 8);
	}
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
