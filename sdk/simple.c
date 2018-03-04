#include <stdint.h>

#define SVC(code) __asm__ __volatile__ ("svc %0" : : "I" (code) )

int fib(int);
uint32_t rand();
void entry();
static uint32_t z1 = 12345;
static uint32_t z2 = 12345;
static uint32_t z3 = 12345;
static uint32_t z4 = 12345;
const char* hello = "Hello World This Is A Program";
extern char _heap_start;


void _start() {
	// zero bss
	extern uint32_t* __bss_start__;
	extern uint32_t* __bss_end__;
	for (uint8_t* dst = (uint8_t*)__bss_start__; dst < (uint8_t*)__bss_end__; dst++) {
		*dst = 0;
	}
	entry();
}

void entry() {
	volatile uint32_t* a = (uint32_t*)&_heap_start;
	*a = 0x50414548;
	uint32_t i = 0;
	while(1) {
		uint32_t r = rand();
		a[i] = r;
		i = (r+fib(i % 8)) % 16;
	}
};

int fib(int n){
	if ( n == 0 ) {
		return 0;
	} else if ( n == 1 ) {
		return 1;
	} else {
		return (fib(n-1) + fib(n-2));
	}
}


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
