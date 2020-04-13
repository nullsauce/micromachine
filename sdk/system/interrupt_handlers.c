
#include "include/interrupt_handlers.h"
#include "include/instructions.h"
#include "include/tinyprintf.h"
#include "include/startup.h"

#define INTERRUPT_USER_OVERRIDE(default_impl) __attribute__((weak,interrupt,alias(#default_impl)))
#define INTERRUPT_UNHANDLED INTERRUPT_USER_OVERRIDE(_isr_unhandled)

INTERRUPT_DEFAULT_IMPL
void _isr_unhandled(void) {}

INTERRUPT_DEFAULT_IMPL
void _isr_reset(void) {

	// This is the entry point
	_startup();
}

INTERRUPT_DEFAULT_IMPL
void _isr_nmi(void) {

}

INTERRUPT_DEFAULT_IMPL
void _isr_hardfault(void) {
	// peek the return address from the stack into r0
	register uint32_t stack __asm__("sp");
	printf("ISR HARDFAULT %08x\n", stack);
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
