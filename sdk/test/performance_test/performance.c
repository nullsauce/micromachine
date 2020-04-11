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


int fib(int n){
	if ( n == 0 ) {
		return 0;
	} else if ( n == 1 ) {
		return 1;
	} else {
		return (fib(n-1) + fib(n-2));
	}
}

void main() {
	volatile uint32_t* heap = (uint32_t*)_system_heap_start;
	for(uint32_t i = 0; i < 10240; i++) {
		heap[rand32() % 16] %= fib(rand32() % 16);
		heap[rand32() % 16] ^= heap[rand32() % 16];
		heap[rand32() % 16] *= heap[rand32() % 16];
		heap[rand32() % 16] -= heap[rand32() % 16];
		heap[rand32() % 16] += heap[rand32() % 16];
		heap[rand32() % 16] /= (rand32() % 4);
	}
}


