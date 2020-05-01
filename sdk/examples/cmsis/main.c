
#include <tinyprintf.h>
#include <micromachine_mcu.h>


void main() {
	printf("Hello cmsis world!\n");

	// External peripherals
	//  TODO USART has to be mapped in the vm
	USART_TypeDef* usart = USART0;
//	usart->CR1 = 2;

	// internal register
	//  TODO SCB has to be mapped in the vm
//	SCB_Type* system_control_block = SCB;
//	uint32_t aircr = system_control_block->AIRCR;

	// internal peripheral
	SysTick_Type *systick = SysTick;
	systick->LOAD = 1000;

	// NVIC
	NVIC_Type *nvic = NVIC;
	uint32_t icpr = nvic->ISER[0];

	NVIC_EnableIRQ(USART_IRQn);
}

