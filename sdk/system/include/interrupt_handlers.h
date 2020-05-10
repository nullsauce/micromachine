
#ifndef MICROMACHINE_EMU_INTERRUPT_HANDLERS_H
#define MICROMACHINE_EMU_INTERRUPT_HANDLERS_H


#define INTERRUPT_HANDLER __attribute__((interrupt, used))

INTERRUPT_HANDLER void _isr_reset(void);
INTERRUPT_HANDLER void _isr_nmi(void);
INTERRUPT_HANDLER void _isr_hardfault(void);
INTERRUPT_HANDLER void _isr_svcall(void);
INTERRUPT_HANDLER void _isr_pendsv(void);

void _isr_systick(void);
void _isr_usart(void);

#endif //MICROMACHINE_EMU_INTERRUPT_HANDLERS_H
