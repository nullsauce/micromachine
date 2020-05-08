

#include <stdint.h>
#include <system.h>
#include <stdio.h>

static uint32_t val = 0xdeadbabe;

int main() {
	val++;
	printf("Hello World!\n");
	printf("System memory layout:\n");
	printf(" * heap start        = %08x\n", _section_heap_start);
	printf(" * heap end          = %08x\n", _section_heap_end);
	printf(" * heap size         = %08x\n", _section_heap_size);
	printf(" * bss start         = %08x\n", _section_bss_start);
	printf(" * bss end           = %08x\n", _section_bss_end);
	printf(" * bss size          = %08x\n", _section_bss_size);
	printf(" * data start        = %08x\n", _section_data_start);
	printf(" * data end          = %08x\n", _section_data_end);
	printf(" * data size         = %08x\n", _section_data_size);
	printf(" * init data start   = %08x\n", _section_init_data_start);
	printf(" * init data end     = %08x\n", _section_init_data_end);
	printf(" * init data size    = %08x\n", _section_init_data_size);

}

