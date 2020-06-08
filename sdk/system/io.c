
#include "include/control_registers.h"
#include "include/instructions.h"
#include "include/system.h"

#include <stdint.h>
#include <stddef.h>
#include <sys/stat.h>

int _write(int fd, char* buf, int count) {
	int written = 0;
	for(; count != 0; --count) {
		IO_REG = (uint32_t)(*buf++);
		++written;
	}
	return written;
}

int _read(int fd, char* buf, int count) {
	return -1;

	int read = 0;

	for(; count > 0; --count) {
		*buf++ = (uint8_t)IO_REG;
		read++;
	}

	return read;
}

void* _sbrk(int incr) {
	static unsigned char* heap = _section_heap_start;
	unsigned char* prev_heap = heap;
	heap += incr;
	return prev_heap;
}

int _fstat(int file, struct stat* st) {
	st->st_mode = S_IFCHR;
	return 0;
}

int _lseek(int file, int offset, int whence) {
	return 0;
}

int _close(int fd) {
	return -1;
}

void _kill(int pid, int sig) {
	return;
}

int _getpid(void) {
	return -1;
}

int _isatty(int file) {
	return 1;
}

int _exit() {
	breakpoint(1);
}
