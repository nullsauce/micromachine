/**************************************************************************//**
 * @file     micromachine_mcu.h
 * @brief    CMSIS Cortex-M0 Core Peripheral Access Layer Header File for
 *           Device micromachine_mcu
 * @version  V5.00
 * @date     10. January 2018
 ******************************************************************************/
/*
 * Copyright (c) 2009-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MICROMACHINE_MCU_H
#define MICROMACHINE_MCU_H


/** @addtogroup Configuration_of_CMSIS
  * @{
  */


/* =========================================================================================================================== */
/* ================                                Interrupt Number Definition                                ================ */
/* =========================================================================================================================== */

typedef enum IRQn
{
/* =======================================  ARM Cortex-M0 Specific Interrupt Numbers  * ======================================== */

	Reset_IRQn                = -15,              /*!< -15  Reset Vector, invoked on Power up and warm reset                     */
	NonMaskableInt_IRQn       = -14,              /*!< -14  Non maskable Interrupt, cannot be stopped or preempted               */
	HardFault_IRQn            = -13,              /*!< -13  Hard Fault, all classes of Fault                                     */
	SVCall_IRQn               =  -5,              /*!< -5 System Service Call via SVC instruction                                */
	PendSV_IRQn               =  -2,              /*!< -2 Pendable request for system service                                    */
	SysTick_IRQn              =  -1,              /*!< -1 System Tick Timer                                                      */

/* ===========================================  micromachine_mcu Specific Interrupt Numbers  ========================================= */
	USART_IRQn                = 0,                /*!< Device Interrupt  */
} IRQn_Type;



/* =========================================================================================================================== */
/* ================                           Processor and Core Peripheral Section                           ================ */
/* =========================================================================================================================== */

/* ===========================  Configuration of the Arm Cortex-M4 Processor and Core Peripherals  =========================== */
/**
  * @brief Configuration of the Cortex-M0+ Processor and Core Peripherals
  */
#define __CM0PLUS_REV             0U /*!< Core Revision r0p0                                   */
#define __MPU_PRESENT             0U /*!< micromachine_mcu  provides no MPU                    */
#define __VTOR_PRESENT            0U /*!< Vector  Table  Register not supported                */
#define __NVIC_PRIO_BITS          2U /*!< micromachine_mcu uses 2 Bits for the Priority Levels */
#define __Vendor_SysTickConfig    0U /*!< Set to 1 if different SysTick Config is used         */


/** @} */ /* End of group Configuration_of_CMSIS */

/**
 * Doesn't really useful for our vm for now
 *
 * (it should define SystemInit which is called by reset_handler at startup)
 * See CMSIS vendor template
 */


/* ========================================  Start of section using anonymous unions  ======================================== */
#if   defined (__CC_ARM)
#pragma push
  #pragma anon_unions
#elif defined (__ICCARM__)
#pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc11-extensions"
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined (__GNUC__)
/* anonymous unions are enabled by default */
#elif defined (__TMS470__)
/* anonymous unions are enabled by default */
#elif defined (__TASKING__)
#pragma warning 586
#elif defined (__CSMC__)
/* anonymous unions are enabled by default */
#else
#warning Not supported compiler type
#endif


/* =========================================================================================================================== */
/* ================                            Device Specific Peripheral Section                             ================ */
/* =========================================================================================================================== */



/** @addtogroup Exported_macros
  * @{
  */
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define READ_BIT(REG, BIT)    ((REG) & (BIT))

#define CLEAR_REG(REG)        ((REG) = (0x0))

#define WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define READ_REG(REG)         ((REG))

#define MODIFY_REG(REG, CLEARMASK, SETMASK)  WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define POSITION_VAL(VAL)     (__CLZ(__RBIT(VAL)))


/**
  * @}
  */ /* End of group Exported_macros */
/** @} */ /* End of group micromachine_mcu */

/** @} */ /* End of group micromachine */


#endif  /* MICROMACHINE_MCU_H */
