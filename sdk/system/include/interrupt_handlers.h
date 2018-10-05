
#ifndef MICROMACHINE_EMU_INTERRUPT_HANDLERS_H
#define MICROMACHINE_EMU_INTERRUPT_HANDLERS_H


#define INTERRUPT_DEFAULT_IMPL __attribute__((interrupt, used))

INTERRUPT_DEFAULT_IMPL void _isr_reset(void);
INTERRUPT_DEFAULT_IMPL void _isr_nmi(void);
INTERRUPT_DEFAULT_IMPL void _isr_hardfault(void);
INTERRUPT_DEFAULT_IMPL void _isr_svcall(void);
INTERRUPT_DEFAULT_IMPL void _isr_pendsv(void);

void _isr_systick(void);
void _isr_external_interrupt(void);

#endif //MICROMACHINE_EMU_INTERRUPT_HANDLERS_H
