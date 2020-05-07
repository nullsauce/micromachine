


#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "md5.h"

void print_hex(uint8_t* buffer, size_t size) {
	const char* hex = "0123456789abcdef";
	for(size_t i = 0; i < size; i++) {
		uint8_t byte = buffer[i];
		char lo = hex[(byte >> 0) & 0xf];
		char hi = hex[(byte >> 4) & 0xf];
		printf("%c%c", hi, lo);
	}
}

void main() {
	MD5_CTX ctx = {0};
	MD5_Init(&ctx);
	const char* data = "md5";
	MD5_Update(&ctx, data, 3);

	uint8_t digest[16];
	MD5_Final(digest, &ctx);
	printf("md5 of '%s' is '", data);
	print_hex(digest, 16);
	printf("'\n");
}

