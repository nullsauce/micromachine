
//


//

#include "MachineTestHarness.hpp"

namespace micromachine::testing {

void MachineTestHarness::SetUp() {

	// point the code generator to the memory
	_code_generator.setMem(&_machine.getCpu().mem());

	// assembler will start emitting instructions at the initial code position
	_code_generator.setWriteAddress(TestMachine::INITIAL_PC);
}

void MachineTestHarness::emitInstruction16(const char* encoding, ...) {
	va_list args;
	va_start(args, encoding);
	_code_generator.emit16bitVaList(encoding, args);
	va_end(args);
}

void MachineTestHarness::emitInstruction32(const char* encoding1, const char* encoding2, ...) {
	va_list args;
	va_start(args, encoding2);
	_code_generator.emit32bitVaList(encoding1, encoding2, args);
	va_end(args);
}

void MachineTestHarness::setNextEmitInstructionAddress(uint32_t address) {
	_code_generator.setWriteAddress(address);
}

void MachineTestHarness::MachineTestHarness::setReg(reg_idx regIdx, uint32_t value) {
	_machine.getCpu().regs().set(regIdx, value);
}

uint32_t MachineTestHarness::getReg(reg_idx regIdx) {
	return _machine.getCpu().regs().get(regIdx);
}

void MachineTestHarness::setNegativeFlag(bool flag) {
	_machine.getCpu().special_regs().app_status_register().negative_flag() = flag;
}

void MachineTestHarness::setZeroFlag(bool flag) {
	_machine.getCpu().special_regs().app_status_register().zero_flag() = flag;
}

void MachineTestHarness::setCarryFlag(bool flag) {
	_machine.getCpu().special_regs().app_status_register().carry_flag() = flag;
}

void MachineTestHarness::setOverflowFlag(bool flag) {
	_machine.getCpu().special_regs().app_status_register().overflow_flag() = flag;
}

void MachineTestHarness::memWrite32(uint32_t address, uint32_t value) {
	_machine.getCpu().mem().write32(address, value);
}

uint32_t MachineTestHarness::memRead32(uint32_t address) {
	return _machine.getCpu().mem().read32_unchecked(address);
}

void MachineTestHarness::resetMachine(uint32_t initialPc) {
	_machine.getMCU().reset(initialPc);
}

void MachineTestHarness::raiseHardfault() {
	_machine.getMCU().exceptions().raise_hardfault();
}

void MachineTestHarness::raiseException(exception ex) {
	_machine.getMCU().exceptions().set_pending(ex, true);
}

void MachineTestHarness::raiseExternalInterrupt(exception ex) {
	assert(ex >= exception::EXTI_00 && "Can't raise an interrupt lower than EXTI_00");
	_machine.getMCU().get_cpu().interrupt().raise_external_interrupt(ex);
}

void MachineTestHarness::enabledAndRaiseExternalInterrupt(exception ex) {
	_machine.getMCU().exceptions().set_enable(ex, true);
	raiseException(ex);
}

void MachineTestHarness::setExceptionPriority(exception ex, exception::priority_t priority) {
	_machine.getMCU().exceptions().set_priority(ex, priority);
}

void MachineTestHarness::setXPSR(uint32_t xpsr) {
	_machine.getMCU().get_cpu().special_regs().xpsr_register() = xpsr;
}

void MachineTestHarness::enterHandlerMode() {
	_machine.getCpu().set_execution_mode(execution_mode::handler);
}

void MachineTestHarness::enterThreadMode() {
	_machine.getCpu().set_execution_mode(execution_mode::thread);
}

void MachineTestHarness::setPrimaskFlag(bool flag) {
	_machine.getCpu().special_regs().primask_register().pm() = flag;
}

void MachineTestHarness::setControl(bool nPriv, bool spSel) {
	_machine.getCpu().special_regs().control_register().n_priv() = nPriv;
	_machine.getCpu().special_regs().control_register().sp_sel() = spSel;
}

}
