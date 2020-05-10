
#include "include/interrupt_handlers.h"
#include "include/instructions.h"
#include "include/startup.h"
#include <stdio.h>

#define INTERRUPT_USER_OVERRIDE(default_impl) __attribute__((weak,interrupt,alias(#default_impl)))
#define INTERRUPT_OVERRIDABLE INTERRUPT_USER_OVERRIDE(_isr_unhandled)

extern void _startup();

INTERRUPT_HANDLER
void _isr_unhandled(void) {}

INTERRUPT_HANDLER
void _isr_reset(void) {
	// This is the entry point
	_startup();
}

INTERRUPT_HANDLER
void _isr_nmi(void) {

}

INTERRUPT_HANDLER
void _isr_hardfault(void) {
	printf("HardFault\n");
	breakpoint(0);
}

INTERRUPT_OVERRIDABLE
void _isr_svcall(void);

INTERRUPT_OVERRIDABLE
void _isr_pendsv(void);

INTERRUPT_OVERRIDABLE
void _isr_systick(void);

INTERRUPT_OVERRIDABLE
void _isr_usart(void);
