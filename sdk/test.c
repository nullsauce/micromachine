

__attribute__((naked))
void _unhandled_exception(void) {
    for(;;);
}

#define UNHANDLED __attribute__((weak, alias("_unhandled_exception")))

UNHANDLED void NMI_Handler(void);
UNHANDLED void HardFault_Handler(void);
UNHANDLED void SVCall_Handler(void);
UNHANDLED void PendSV_Handler(void);
UNHANDLED void SysTick_Handler(void);

UNHANDLED void SPI0_IRQ(void);
UNHANDLED void SPI1_IRQ(void);
UNHANDLED void UART0_IRQ(void);
UNHANDLED void UART1_IRQ(void);
UNHANDLED void UART2_IRQ(void);
UNHANDLED void I2C0_IRQ(void);
UNHANDLED void SCT_IRQ(void);
UNHANDLED void MRT_IRQ(void);
UNHANDLED void CMP_IRQ(void);
UNHANDLED void WDT_IRQ(void);
UNHANDLED void BOD_IRQ(void);
UNHANDLED void WKT_IRQ(void);
UNHANDLED void PININT0_IRQ(void);
UNHANDLED void PININT1_IRQ(void);
UNHANDLED void PININT2_IRQ(void);
UNHANDLED void PININT3_IRQ(void);
UNHANDLED void PININT4_IRQ(void);
UNHANDLED void PININT5_IRQ(void);
UNHANDLED void PININT6_IRQ(void);
UNHANDLED void PININT7_IRQ(void);


void reset() {
  int a = 2;
  int b = 7;
  volatile int* p = (volatile int*)0xc0ffee;
  *p = a + b;
  return;
}