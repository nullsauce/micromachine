
#include "interrupt_handlers.h"
#include "instructions.h"
#include "tinyprintf.h"

INTERRUPT_DEFAULT_IMPL void _isr_unhandled(void) {}
INTERRUPT_DEFAULT_IMPL void _isr_reset(void) {}
INTERRUPT_DEFAULT_IMPL void _isr_nmi(void) {}
INTERRUPT_DEFAULT_IMPL void _isr_hardfault(void) {
	printf("HARDFAULT\n");
	breakpoint(0);
}
INTERRUPT_DEFAULT_IMPL void _isr_svcall(void) {}
INTERRUPT_DEFAULT_IMPL void _isr_pendsv(void) {}
INTERRUPT_DEFAULT_IMPL void _isr_systick(void) {}
INTERRUPT_DEFAULT_IMPL void _isr_external_interrupt(void) {}

