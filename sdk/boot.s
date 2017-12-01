/*
This is the startup file for LPC81x. The C definition of the vector tables are defined in vector.c
*/
.thumb
.section .text

.align 2

/* vector  */
.long _stack_start
.long handle_reset
.long NMI_Handler
.long HardFault_Handler
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long 0
.long SVCall_Handler
.long 0
.long 0
.long PendSV_Handler
.long SysTick_Handler
.long 0

/* IRQ 0 */
.long SPI0_IRQ
.long SPI1_IRQ
.long 0
.long UART0_IRQ
.long UART1_IRQ
.long UART2_IRQ
.long 0
.long 0
/* IRQ 8 */
.long I2C0_IRQ
.long SCT_IRQ
.long MRT_IRQ
.long CMP_IRQ
.long WDT_IRQ
.long BOD_IRQ
.long 0
.long WKT_IRQ
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
.long PININT0_IRQ
.long PININT1_IRQ
.long PININT2_IRQ
.long PININT3_IRQ
.long PININT4_IRQ
.long PININT5_IRQ
.long PININT6_IRQ
.long PININT7_IRQ

.text
.align

.global handle_reset
handle_reset:
    b reset
