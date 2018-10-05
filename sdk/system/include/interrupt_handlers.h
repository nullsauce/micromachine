
#ifndef MICROMACHINE_EMU_INTERRUPT_HANDLERS_H
#define MICROMACHINE_EMU_INTERRUPT_HANDLERS_H

// system provided implementations
#define INTERRUPT_DEFAULT_IMPL __attribute__((interrupt, used))
#define INTERRUPT_UNHANDLED __attribute__((weak,interrupt,alias("_isr_unhandled")))

INTERRUPT_DEFAULT_IMPL void _isr_unhandled(void);
INTERRUPT_DEFAULT_IMPL void _isr_reset(void);
INTERRUPT_DEFAULT_IMPL void _isr_nmi(void);
INTERRUPT_DEFAULT_IMPL void _isr_hardfault(void);
INTERRUPT_DEFAULT_IMPL void _isr_svcall(void);
INTERRUPT_DEFAULT_IMPL void _isr_pendsv(void);
INTERRUPT_DEFAULT_IMPL void _isr_systick(void);
INTERRUPT_DEFAULT_IMPL void _isr_external_interrupt(void);

// user providable override
#define INTERRUPT_USER_OVERRIDE(default_impl) __attribute__((weak,interrupt,alias(#default_impl)))


#endif //MICROMACHINE_EMU_INTERRUPT_HANDLERS_H
