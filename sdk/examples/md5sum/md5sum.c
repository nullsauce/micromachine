

#include <interrupt_handlers.h>
#include <instructions.h>
#include <systick.h>
#include <stdint.h>
#include <stddef.h>
#include <tinyprintf.h>
#include <control_registers.h>
#include "md5.h"


extern char _heap_start;

void io_call(uint8_t op, uint8_t d0, uint8_t d1, uint8_t d2) {
	IO_REG = ((op << 24) | (d2 << 16) | (d2 << 8) | (d0 << 0));
}

void printf_putc(void* ptr, char c) {
	io_call(0, c, 0, 0);
}


void print_hex(uint8_t* buffer, size_t size) {
	const char* hex = "0123456789abcdef";
	for(size_t i = 0; i < size; i++) {
		uint8_t byte = buffer[i];
		char lo = hex[(byte >> 0) & 0xf];
		char hi = hex[(byte >> 4) & 0xf];
		printf_putc(0, hi);
		printf_putc(0, lo);
	}
}

void main() {
	MD5_CTX ctx = {0};
	init_printf(NULL, printf_putc);
	MD5_Init(&ctx);
	const char* data = "md5";
	MD5_Update(&ctx, data, 3);
	MD5_Final(&_heap_start, &ctx);
	printf("md5 of '%s' is '", data);
	print_hex(&_heap_start, 16);
	printf("'\n");
}

