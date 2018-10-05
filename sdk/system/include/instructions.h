
#ifndef MICROMACHINE_EMU_INSTRUCTIONS_H
#define MICROMACHINE_EMU_INSTRUCTIONS_H

#define breakpoint(code) __asm__ __volatile__ ("bkpt %0" : : "I" (code) )
#define svc(code) __asm__ __volatile__ ("svc %0" : : "I" (code) )

#endif //MICROMACHINE_EMU_INSTRUCTIONS_H
