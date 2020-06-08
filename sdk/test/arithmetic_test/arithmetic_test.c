

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

static uint32_t z1 = 0x2f312e94;
static uint32_t z2 = 0xfebf9a50;
static uint32_t z3 = 0xbdd3a43c;
static uint32_t z4 = 0x41864de9;

uint32_t rand32() {
	uint32_t b;
	b = ((z1 << 6) ^ z1) >> 13;
	z1 = ((z1 & 4294967294U) << 18) ^ b;
	b = ((z2 << 2) ^ z2) >> 27;
	z2 = ((z2 & 4294967288U) << 2) ^ b;
	b = ((z3 << 13) ^ z3) >> 21;
	z3 = ((z3 & 4294967280U) << 7) ^ b;
	b = ((z4 << 3) ^ z4) >> 12;
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

#define MAX_FIB_DEPTH 8
#define NUM_ITERATIONS 256

void main() {

	uint32_t values[16];

	for(int i = 0; i < 16; i++) {
		values[i] = rand32();
	}

	for(int i = 0; i < 16; i++) {
		printf("%08x\n", values[i]);
	}

	printf("--------\n");
	for(uint32_t i = 0; i < NUM_ITERATIONS; i++) {
		values[rand32() % 16] ^= values[rand32() % 16];
		values[rand32() % 16] /= 1 + fib(rand32() % MAX_FIB_DEPTH);
		values[rand32() % 16] *= fib(rand32() % MAX_FIB_DEPTH);
		values[rand32() % 16] += (fib(rand32() % MAX_FIB_DEPTH) * 0xff) % (1+fib(rand32() % MAX_FIB_DEPTH));
		values[rand32() % 16] -= values[rand32() % 16];
		values[rand32() % 16] += values[rand32() % 16];
		values[rand32() % 16] += fib(values[rand32() % MAX_FIB_DEPTH] % 16);
	}

	for(int i = 0; i < 16; i++) {
		printf("%08x\n", values[i]);
	}
}


