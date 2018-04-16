//
// Created by fla on 17-5-20.
//

#ifndef MICROMACHINE_PINKSIM_HPP
#define MICROMACHINE_PINKSIM_HPP


/* Bits in PinkySimContext::xPSR */
#define APSR_N      (1U << 31U) /* Negative flag */
#define APSR_Z      (1 << 30)   /* Zero flag */
#define APSR_C      (1 << 29)   /* Carry flag */
#define APSR_V      (1 << 28)   /* Overflow flag */
#define IPSR_MASK   0x3F        /* Mask for exception number */
#define EPSR_T      (1 << 24)   /* Thumb mode flag */

/* Useful xPSR flag combinations for masking off at runtime. */
#define APSR_NZCV   (APSR_N | APSR_Z | APSR_C | APSR_V)
#define APSR_NZC    (APSR_N | APSR_Z | APSR_C)
#define APSR_NZ     (APSR_N | APSR_Z)
#define APSR_NC     (APSR_N | APSR_C)
#define APSR_ZC     (APSR_Z | APSR_C)

/* Bits in PinkySimContext::PRIMASK */
#define PRIMASK_PM (1 << 0)

/* Condition codes */
#define COND_EQ 0x0
#define COND_NE (COND_EQ | 1)
#define COND_CS 0x2
#define COND_CC (COND_CS | 1)
#define COND_MI 0x4
#define COND_PL (COND_MI | 1)
#define COND_VS 0x6
#define COND_VC (COND_VS | 1)
#define COND_HI 0x8
#define COND_LS (COND_HI | 1)
#define COND_GE 0xA
#define COND_LT (COND_GE | 1)
#define COND_GT 0xC
#define COND_LE (COND_GT | 1)
#define COND_AL 0xE

/* SYSm field values for MSR and MRS instructions. */
#define SYS_APSR    0
#define SYS_IAPSR   1
#define SYS_EAPSR   2
#define SYS_XPSR    3
#define SYS_IPSR    5
#define SYS_EPSR    6
#define SYS_IEPSR   7
#define SYS_MSP     8
#define SYS_PSP     9
#define SYS_PRIMASK 16
#define SYS_CONTROL 20

/* Register names / indices into PinkySimContext::R array for first 13 registers. */
#define R0  0
#define R1  1
#define R2  2
#define R3  3
#define R4  4
#define R5  5
#define R6  6
#define R7  7
#define R8  8
#define R9  9
#define R10 10
#define R11 11
#define R12 12
#define SP  13
#define LR  14
#define PC  15


/* Values that can be returned from the pinkySimStep() or pinkySimRun() function. */
#define PINKYSIM_STEP_OK            0   /* Executed instruction successfully. */
#define PINKYSIM_STEP_UNDEFINED     1   /* Encountered undefined instruction. */
#define PINKYSIM_STEP_UNPREDICTABLE 2   /* Encountered instruction with unpredictable behaviour. */
#define PINKYSIM_STEP_HARDFAULT     3   /* Encountered instruction which generates hard fault. */
#define PINKYSIM_STEP_BKPT          4   /* Encountered BKPT instruction or other debug event. */
#define PINKYSIM_STEP_UNSUPPORTED   5   /* Encountered instruction not supported by simulator. */
#define PINKYSIM_STEP_SVC           11   /* Encountered SVC instruction. */
#define PINKYSIM_RUN_INTERRUPT      7   /* pinkySimRun() callback signalled interrupt. */
#define PINKYSIM_RUN_WATCHPOINT     8   /* pinkySimRun() callback signalled watchpoint event. */
#define PINKYSIM_RUN_SINGLESTEP     9   /* pinkySimRun() callback signalled single step. */

#endif //MICROMACHINE_PINKSIM_HPP
