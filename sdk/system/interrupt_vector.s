/*
This is the startup file for LPC81x. The C definition of the vector tables are defined in vector.c
*/
.thumb
.force_thumb
.section .isr_vector, "ax"

// LD variables
.extern _stack_bottom

// ISR handlers
.extern _isr_reset
.extern _isr_nmi
.extern _isr_hardfault
.extern _isr_svcall
.extern _isr_pendsv
.extern _isr_systick
// external interrupts
.extern _isr_usart


.align 2

// Interrupt vector table
.long _stack_bottom
.long _isr_reset
.long _isr_nmi
.long _isr_hardfault
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long _isr_svcall
.long 0
.long 0
.long _isr_pendsv
.long _isr_systick

/* IRQ 0 */
.long _isr_usart // mapped to exti_00
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
/* IRQ 8 */
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
/* IRQ 16 */
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
/* IRQ 24 */
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0

