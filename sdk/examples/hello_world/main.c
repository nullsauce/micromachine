
#include <tinyprintf.h>
#include <stdint.h>
#include <control_registers.h>
#include <system.h>

void io_call(uint8_t op, uint8_t d0, uint8_t d1, uint8_t d2) {
	IO_REG = ((op << 24) | (d2 << 16) | (d2 << 8) | (d0 << 0));
}

void printf_putc(void* ptr, char c) {
	io_call(0, c, 0, 0);
}

void main() {
	// bind the printf function to the io_call interface
	init_printf(NULL, printf_putc);
	printf("Hello World!\n");
	printf("System memory layout:\n");
	printf(" * heap start = %08x\n", _system_heap_start);
	printf(" * heap end   = %08x\n", _system_heap_end);
	printf(" * heap size  = %08x\n", _system_heap_size);
	printf(" * bss start  = %08x\n", _system_bss_start);
	printf(" * bss end    = %08x\n", _system_bss_end);
	printf(" * bss size   = %08x\n", _system_bss_size);

}

