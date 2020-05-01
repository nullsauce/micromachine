
//
// Created by fla on 30.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#include "mcu.hpp"
#include "TestHelpers.hpp"
#include <gtest/gtest.h>

using namespace micromachine::system;

namespace {

class MachineTestHarness : public ::testing::Test {
protected:
	mcu _machine;
};

class USARTControllerTestHarness : public MachineTestHarness {
protected:
	usart_controller& usart() {
		return _machine.get_usart_controller();
	}

	void enableUSART() {
		binops::set_bit(usart().control_register(), usart_cr1_reg::enable_bit::offset);
	}

	void disableUSART() {
		binops::clear_bit(usart().control_register(), usart_cr1_reg::enable_bit::offset);
	}

	void SetUp() override {
		usart().reset();
		enableUSART();
	}
};

}

MICROMACHINE_TEST_F(USART_PeripheralInit, CR1InitallyDisabled, MachineTestHarness) {
	usart_controller& controller = _machine.get_usart_controller();
	controller.reset();
	EXPECT_FALSE(controller.control_register().enabled());
}

MICROMACHINE_TEST_F(USART_Peripheral, CR1Enable, USARTControllerTestHarness) {
	EXPECT_TRUE(usart().control_register().enabled());
}

MICROMACHINE_TEST_F(USART_Peripheral, CR1ClearEnableResetsISRStatusFlags, USARTControllerTestHarness) {
	usart().interrupt_status_register().set_rx_not_empty(true);
	usart().interrupt_status_register().set_tx_empty(false);
	binops::clear_bit(usart().control_register(), usart_cr1_reg::enable_bit::offset);
	EXPECT_FALSE(usart().interrupt_status_register().rx_not_empty());
}

MICROMACHINE_TEST_F(USART_Peripheral, ResetResetsCR1EnableFlag, USARTControllerTestHarness) {
	usart().reset();
	EXPECT_FALSE(usart().control_register().enabled());
}

MICROMACHINE_TEST_F(USART_Peripheral, ISRRxNotEmptyInitallyClear, USARTControllerTestHarness) {
	EXPECT_FALSE(usart().interrupt_status_register().rx_not_empty());
}

MICROMACHINE_TEST_F(USART_Peripheral, ISRTxEmptyInitallySet, USARTControllerTestHarness) {
	EXPECT_TRUE(usart().interrupt_status_register().tx_empty());
}

MICROMACHINE_TEST_F(USART_Peripheral, ISRTxEmptyClearedByAWriteToTxReg, USARTControllerTestHarness) {
	usart().tx_register() = 0xcd;
	EXPECT_FALSE(usart().interrupt_status_register().tx_empty());
}

MICROMACHINE_TEST_F(USART_Peripheral, ISRTxEmptySetByAReadToShiftRegister, USARTControllerTestHarness) {
	usart().tx_register() = 0xcd;
	uint8_t readByte = usart().tx_register().read();
	EXPECT_TRUE(usart().interrupt_status_register().tx_empty());
	EXPECT_EQ(0xcd, readByte);
}

MICROMACHINE_TEST_F(USART_Peripheral, ISRRxNotEmptySetByAWriteFromShiftRegister, USARTControllerTestHarness) {
	usart().rx_register().write(0xee);
	EXPECT_TRUE(usart().interrupt_status_register().rx_not_empty());
}

MICROMACHINE_TEST_F(USART_Peripheral, ISRRxNotEmptyClearedByAReadOfRxReg, USARTControllerTestHarness) {
	usart().rx_register().write(0xee);
	uint32_t readWord = usart().rx_register();
	EXPECT_FALSE(usart().interrupt_status_register().rx_not_empty());
	EXPECT_EQ(0xee, readWord);
}

MICROMACHINE_TEST_F(USART_Peripheral, ICRClearsTxEmptyFlag, USARTControllerTestHarness) {
	usart().interrupt_status_register().set_tx_empty(true);
	usart().interrupt_status_register().set_rx_not_empty(true);
	binops::set_bit(usart().interrupt_clear_register(), usart_ic_reg::clear_tx_empty_bit::offset);
	EXPECT_FALSE(usart().interrupt_status_register().tx_empty());
	EXPECT_TRUE(usart().interrupt_status_register().rx_not_empty());
}

MICROMACHINE_TEST_F(USART_Peripheral, ICRClearsRxNotEmptyFlag, USARTControllerTestHarness) {
	usart().interrupt_status_register().set_tx_empty(true);
	usart().interrupt_status_register().set_rx_not_empty(true);
	binops::set_bit(usart().interrupt_clear_register(), usart_ic_reg::clear_rx_not_empty_bit::offset);
	EXPECT_TRUE(usart().interrupt_status_register().tx_empty());
	EXPECT_FALSE(usart().interrupt_status_register().rx_not_empty());
}

MICROMACHINE_TEST_F(USART_Peripheral, ICRClearsBothTxAndRxEmptyFlags, USARTControllerTestHarness) {
	usart().interrupt_status_register().set_tx_empty(true);
	usart().interrupt_status_register().set_rx_not_empty(true);
	usart().interrupt_clear_register() = usart().interrupt_clear_register()
		 | 1 << usart_ic_reg::clear_tx_empty_bit::offset
		 | 1 << usart_ic_reg::clear_rx_not_empty_bit::offset;
	EXPECT_FALSE(usart().interrupt_status_register().tx_empty());
	EXPECT_FALSE(usart().interrupt_status_register().rx_not_empty());
}

MICROMACHINE_TEST_F(USART_Peripheral, StepMovesTxRegisterIntoTxBuffer, USARTControllerTestHarness) {
	usart().tx_register() = 0xcd;
	usart().step();
	uint8_t expectedTxData = 0xcd;
	uint8_t actualData = 0;
	usart().receive(actualData);
	EXPECT_EQ(expectedTxData, actualData);
}

MICROMACHINE_TEST_F(USART_Peripheral, StepMovesRxBufferIntoRxRegister, USARTControllerTestHarness) {
	usart().send(0xcd);
	usart().step();
	uint32_t actualData = usart().rx_register();
	uint32_t expectedData = 0x000000cd;
	EXPECT_EQ(expectedData, actualData);
}

MICROMACHINE_TEST_F(USART_Peripheral, StepDoesNotMoveRxBufferIntoRxRegisterIfRxNotEmpty, USARTControllerTestHarness) {
	usart().send(0xcd);
	usart().step();
	usart().send(0xef);
	usart().step();
	EXPECT_EQ(0x000000cd, usart().rx_register());
	EXPECT_EQ(1U, usart().rx_buffer_size());
}

MICROMACHINE_TEST_F(USART_Peripheral, StepDoesEventuallyRxBufferIntoRxRegisterIfRxNotEmpty, USARTControllerTestHarness) {
	usart().send(0xcd);
	usart().step();
	usart().send(0xef);
	usart().step();
	EXPECT_EQ(0x000000cd, usart().rx_register());
	usart().step();
	EXPECT_EQ(0x000000ef, usart().rx_register());
	EXPECT_EQ(0U, usart().rx_buffer_size());
}

MICROMACHINE_TEST_F(USART_Peripheral, StepDoesNotMoveTxRegisterIntoTxBufferWhenDisabled, USARTControllerTestHarness) {
	usart().tx_register() = 0xcd;
	disableUSART();
	usart().step();
	EXPECT_EQ(0, usart().tx_buffer_size());
}

MICROMACHINE_TEST_F(USART_Peripheral, StepDoesNotMoveRxBufferIntoRxRegisterWhenDisabled, USARTControllerTestHarness) {
	usart().send(0xcd);
	disableUSART();
	usart().step();
	EXPECT_FALSE(usart().interrupt_status_register().rx_not_empty());
	EXPECT_EQ(0, usart().rx_register());
}

MICROMACHINE_TEST_F(USART_Peripheral, ICREnableTxEmptyInterruptTriggersAnInterrupt, USARTControllerTestHarness) {
	binops::set_bit(usart().control_register(), usart_cr1_reg::tx_empty_interrupt_enable_bit::offset);
	usart().step();
	EXPECT_TRUE(mcu().get_exception_vector().interrupt_state(exception::EXTI_00).is_pending());
}

MICROMACHINE_TEST_F(USART_Peripheral, ResetDoeNotTriggersAnInterrupt, USARTControllerTestHarness) {
	binops::set_bit(usart().control_register(), usart_cr1_reg::tx_empty_interrupt_enable_bit::offset);
	usart().reset();
	EXPECT_FALSE(_machine.get_exception_vector().interrupt_state(exception::EXTI_00).is_pending());
}

MICROMACHINE_TEST_F(USART_Peripheral, ICREnableRxNotEmptyInterruptTriggersAnInterrupt, USARTControllerTestHarness) {
	usart().send(0xdf);
	usart().step();
	EXPECT_FALSE(_machine.get_exception_vector().interrupt_state(exception::EXTI_00).is_pending());
	binops::set_bit(usart().control_register(), usart_cr1_reg::rx_not_empty_interrupt_enable_bit::offset);
	usart().step();
	EXPECT_TRUE(_machine.get_exception_vector().interrupt_state(exception::EXTI_00).is_pending());
}

MICROMACHINE_TEST_F(USART_Peripheral, ICRDisableRxNotEmptyInterruptDoesNotTriggersAnInterrupt, USARTControllerTestHarness) {
	usart().send(0xdf);
	usart().step();
	EXPECT_FALSE(_machine.get_exception_vector().interrupt_state(exception::EXTI_00).is_pending());
	binops::set_bit(usart().control_register(), usart_cr1_reg::rx_not_empty_interrupt_enable_bit::offset);
	usart().step();
	EXPECT_TRUE(_machine.get_exception_vector().interrupt_state(exception::EXTI_00).is_pending());
	_machine.get_exception_vector().interrupt_state(exception::EXTI_00).set_pending(false);
	usart().step();
	binops::clear_bit(usart().control_register(), usart_cr1_reg::rx_not_empty_interrupt_enable_bit::offset);
	usart().step();
	EXPECT_FALSE(_machine.get_exception_vector().interrupt_state(exception::EXTI_00).is_pending());
}
