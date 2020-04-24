

#include <interrupt_handlers.h>
#include <instructions.h>
#include <systick.h>
#include <stdint.h>
#include <stddef.h>
#include <tinyprintf.h>
#include <control_registers.h>
#include <system.h>
#include <io.h>

#include "md5.h"

void print_hex(uint8_t* buffer, size_t size) {
	const char* hex = "0123456789abcdef";
	for(size_t i = 0; i < size; i++) {
		uint8_t byte = buffer[i];
		char lo = hex[(byte >> 0) & 0xf];
		char hi = hex[(byte >> 4) & 0xf];
		_putc(hi);
		_putc(lo);
	}
}

void main() {
	MD5_CTX ctx = {0};
	MD5_Init(&ctx);
	const char* data = "md5";
	MD5_Update(&ctx, data, 3);
	MD5_Final(_section_heap_start, &ctx);
	printf("md5 of '%s' is '", data);
	print_hex(_section_heap_start, 16);
	printf("'\n");
}

