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
#include <random.h>
#include <system.h>

void io_call(uint8_t op, uint8_t d0, uint8_t d1, uint8_t d2) {
	IO_REG = ((op << 24) | (d2 << 16) | (d2 << 8) | (d0 << 0));
}

void printf_putc(void* ptr, char c) {
	io_call(0, c, 0, 0);
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

#define MAX_FIB_DEPTH 8
#define NUM_ITERATIONS 256

void main() {
	init_printf(NULL, printf_putc);

	volatile uint32_t* heap = (uint32_t*)_system_heap_start;

	for(int i = 0; i < 16; i++) {
		heap[i] = rand32();
	}

	for(int i = 0; i < 16; i++) {
		printf("%08x\n", heap[i]);
	}

	printf("--------\n");
	for(uint32_t i = 0; i < NUM_ITERATIONS; i++) {
		heap[rand32() % 16] ^= heap[rand32() % 16];
		heap[rand32() % 16] /= 1 + fib(rand32() % MAX_FIB_DEPTH);
		heap[rand32() % 16] *= fib(rand32() % MAX_FIB_DEPTH);
		heap[rand32() % 16] += (fib(rand32() % MAX_FIB_DEPTH) * 0xff) % (1+fib(rand32() % MAX_FIB_DEPTH));
		heap[rand32() % 16] -= heap[rand32() % 16];
		heap[rand32() % 16] += heap[rand32() % 16];
		heap[rand32() % 16] += fib(heap[rand32() % MAX_FIB_DEPTH] % 16);
	}

	for(int i = 0; i < 16; i++) {
		printf("%08x\n", heap[i]);
	}
}


