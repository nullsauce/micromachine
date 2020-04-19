//
// Created by joel on 17/04/2020.
//

#ifndef MICROMACHINE_USART_H
#define MICROMACHINE_USART_H

#include <stdint.h>
#include <core_cm0.h>
#include <micromachine_mcu.h>


struct usart_port {
	__IO uint32_t CR1;
	__IO uint32_t ISR;
	__IO uint32_t ICR;
	__IO uint32_t RX;
	__IO uint32_t TX;
};

#define USART ((struct usart_port*)USART_BASE)

/* Usart control register bits definitions  */
#define USART_CR1_UE_Pos          (0U)
#define USART_CR1_UE_Msk          (0x1UL << USART_CR1_UE_Pos)
#define USART_CR1_UE              USART_CR1_UE_Msk               /* USART enable */
#define USART_CR1_RXNE_Pos        (1U)
#define USART_CR1_RXNE_Msk        (0x1UL << USART_CR1_RXNE_Pos)
#define USART_CR1_RXNE            USART_CR1_RXNE_Msk             /* RX not empty interrupt enable */
#define USART_CR1_TXC_Pos         (2U)
#define USART_CR1_TXC_Msk         (0x1UL << USART_CR1_TXC_Pos)
#define USART_CR1_TXC             USART_CR1_TXC_Msk              /* TX complete interrupt enable */
#define USART_CR1_TXE_Pos         (3U)
#define USART_CR1_TXE_Msk         (0x1UL << USART_CR1_TXE_Pos)
#define USART_CR1_TXE             USART_CR1_TXE_Msk              /* TX empty interrupt enable */

/* Usart interrupt status register bits definitions */
#define USART_ISR_TXE_Pos          (0U)
#define USART_ISR_TXE_Msk          (0x1UL << USART_ISR_TXE_Pos)
#define USART_ISR_TXE              USART_ISR_TXE_Msk               /* TX empty interrupt status */
#define USART_ISR_TXC_Pos          (1U)
#define USART_ISR_TXC_Msk          (0x1UL << USART_ISR_TXC_Pos)
#define USART_ISR_TXC              USART_ISR_TXC_Msk              /* TX complete interrupt status */
#define USART_ISR_RXNE_Pos         (2U)
#define USART_ISR_RXNE_Msk         (0x1UL << USART_ISR_RXNE_Pos)
#define USART_ISR_RXNE             USART_ISR_RXNE_Msk              /* RX not empty interrupt status */

/* Usart interrupt clear flag register bits definitions */
#define USART_ICR_TXC_Pos          (0U)
#define USART_ICR_TXC_Msk          (0x1UL << USART_ICR_TXC_Pos)
#define USART_ICR_TXC              USART_ICR_TXC_Msk              /* TX complete interrupt clear */

#endif // MICROMACHINE_USART_H
