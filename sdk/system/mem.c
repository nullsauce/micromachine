
#include "include/mem.h"

void* memset(void* mem, int c, size_t n) {
	char *s = (char*)mem;
	while (n--)
		*s++ = (char)c;
	return mem;
}

void* memcpy(void* __restrict__ dst0, const void* __restrict__ src0, size_t len0) {
	char *dst = (char *) dst0;
	char *src = (char *) src0;
	void* save = dst0;
	while (len0--) {
		*dst++ = *src++;
	}
	return save;
}