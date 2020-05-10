
//
// Created by fla on 10.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#ifndef MICROMACHINE_PERIPHERALS_H
#define MICROMACHINE_PERIPHERALS_H

/** @addtogroup Device_Peripheral_peripherals
  * @{
 */

/* ===========================================================================================================================
 */
/* ================                                            USART ================ */
/* ===========================================================================================================================
 */

/**
  * @brief USART
 */

typedef struct {
	__IO uint32_t CR1; /*!< (@ 0x00000000) Control register 1                      */
	__IO uint32_t ISR; /*!< (@ 0x00000004) Interrupt and status register           */
	__IO uint32_t ICR; /*!< (@ 0x00000008) Interrupt clear register                */
	__IO uint32_t RX;  /*!< (@ 0x0000000c) Receive data register                   */
	__IO uint32_t TX;  /*!< (@ 0x00000010) Transmit data register                  */
} USART_TypeDef;

/*@}*/ /* end of group micromachine_mcu_Peripherals */

/* =========================================  End of section using anonymous unions  ========================================= */
#if defined(__CC_ARM)
#pragma pop
#elif defined(__ICCARM__)
/* leave anonymous unions enabled */
#elif(__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
/* anonymous unions are enabled by default */
#elif defined(__TMS470__)
/* anonymous unions are enabled by default */
#elif defined(__TASKING__)
#pragma warning restore
#elif defined(__CSMC__)
/* anonymous unions are enabled by default */
#else
#warning Not supported compiler type
#endif

/* ===========================================================================================================================
 */
/* ================                          Device Specific Peripheral Address Map ================ */
/* ===========================================================================================================================
 */

/** @addtogroup Device_Peripheral_peripheralAddr
  * @{
 */

/* Peripheral and SRAM base address */
#define FLASH_BASE (0x00000000UL)  /*!< (FLASH     ) Base Address */
#define SRAM_BASE (0x20000000UL)   /*!< (SRAM      ) Base Address */
#define PERIPH_BASE (0x40000000UL) /*!< (Peripheral) Base Address */

/* Peripheral memory map */
#define USART_BASE (PERIPH_BASE) /*!< (USART     ) Base Address */
//#define <next_perip>_BASE (PERIPH_BASE + x) /*!< (<periph>    ) Base Address */

/** @} */ /* End of group Device_Peripheral_peripheralAddr */

/* ===========================================================================================================================
 */
/* ================                                  Peripheral declaration ================ */
/* ===========================================================================================================================
 */

/** @addtogroup Device_Peripheral_declaration
  * @{
 */

#define USART0 ((USART_TypeDef*)USART_BASE)
/** @} */ /* End of group Device_Peripheral_declaration */

/** @addtogroup Peripheral_Registers_Bits_Definition
* @{
 */
/* Usart control register bits definitions  */
#define USART_CR1_UE_Pos (0U)
#define USART_CR1_UE_Msk (0x1UL << USART_CR1_UE_Pos)
#define USART_CR1_UE USART_CR1_UE_Msk /* USART enable */
#define USART_CR1_RXNE_Pos (1U)
#define USART_CR1_RXNE_Msk (0x1UL << USART_CR1_RXNE_Pos)
#define USART_CR1_RXNE USART_CR1_RXNE_Msk /* RX not empty interrupt enable */
#define USART_CR1_TXC_Pos (2U)
#define USART_CR1_TXC_Msk (0x1UL << USART_CR1_TXC_Pos)
#define USART_CR1_TXC USART_CR1_TXC_Msk /* TX complete interrupt enable */
#define USART_CR1_TXE_Pos (3U)
#define USART_CR1_TXE_Msk (0x1UL << USART_CR1_TXE_Pos)
#define USART_CR1_TXE USART_CR1_TXE_Msk /* TX empty interrupt enable */

/* Usart interrupt status register bits definitions */
#define USART_ISR_RXNE_Pos (1U)
#define USART_ISR_RXNE_Msk (0x1UL << USART_ISR_RXNE_Pos)
#define USART_ISR_RXNE USART_ISR_RXNE_Msk /* RX not empty interrupt status */
#define USART_ISR_TXC_Pos (2U)
#define USART_ISR_TXC_Msk (0x1UL << USART_ISR_TXC_Pos)
#define USART_ISR_TXC USART_ISR_TXC_Msk /* TX complete interrupt status */
#define USART_ISR_TXE_Pos (3U)
#define USART_ISR_TXE_Msk (0x1UL << USART_ISR_TXE_Pos)
#define USART_ISR_TXE USART_ISR_TXE_Msk /* TX empty interrupt status */

/* Usart interrupt clear flag register bits definitions */
#define USART_ICR_RXNE_Pos (1U)
#define USART_ICR_RXNE_Msk (0x1UL << USART_ISR_RXNE_Pos)
#define USART_ICR_RXNE USART_ISR_RXNE_Msk /* RX not empty interrupt status */
#define USART_ICR_TXC_Pos (2U)
#define USART_ICR_TXC_Msk (0x1UL << USART_ICR_TXC_Pos)
#define USART_ICR_TXC USART_ICR_TXC_Msk /* TX complete interrupt clear */

/* @} */ /* End of group Peripheral_Registers_Bits_Definition */

#endif
