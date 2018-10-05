
#include "include/interrupt_handlers.h"
#include "include/instructions.h"
#include "include/tinyprintf.h"
#include "include/entrypoint.h"

#define INTERRUPT_USER_OVERRIDE(default_impl) __attribute__((weak,interrupt,alias(#default_impl)))
#define INTERRUPT_UNHANDLED INTERRUPT_USER_OVERRIDE(_isr_unhandled)

INTERRUPT_DEFAULT_IMPL
void _isr_unhandled(void) {}

INTERRUPT_DEFAULT_IMPL
void _isr_reset(void) {
	extern uint32_t* __bss_start__;
	extern uint32_t* __bss_end__;
	for (uint8_t* dst = (uint8_t*)__bss_start__; dst < (uint8_t*)__bss_end__; dst++) {
		*dst = 0;
	}
	main();
	breakpoint(0xff);
}

INTERRUPT_DEFAULT_IMPL
void _isr_nmi(void) {

}

INTERRUPT_DEFAULT_IMPL
void _isr_hardfault(void) {
	// peek the return addres from the stack into r0
	register uint32_t stack asm("sp");
	printf("HARDFAULT %08x\n", stack);
	breakpoint(0);
}

INTERRUPT_DEFAULT_IMPL
void _isr_svcall(void) {

}

INTERRUPT_DEFAULT_IMPL
void _isr_pendsv(void) {

}

INTERRUPT_UNHANDLED
void _isr_systick(void);

INTERRUPT_UNHANDLED
void _isr_external_interrupt(void);