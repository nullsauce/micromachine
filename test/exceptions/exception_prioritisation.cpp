

#include "CpuTestHarness.hpp"
#include "mini_assembler.hpp"


TEST_F(CpuTestHarness, DefaultPriorityIsThreadModeAtStartup)
{
	EXPECT_EQ(exception::THREAD_MODE_PRIORITY, _cpu.current_execution_priority());
}



TEST_F(CpuTestHarness, ExecutionPriorityChangesWhenExceptionIsTaken)
{
	EXPECT_EQ(exception::THREAD_MODE_PRIORITY, _cpu.current_execution_priority());
	_cpu.interrupt().raise_hardfault();
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();
	EXPECT_EQ(exception::HARDFAULT_PRIORITY, _cpu.current_execution_priority());
}

TEST_F(CpuTestHarness, ExecutionPriorityGoesBackToDefaultAfterExceptionIsTaken)
{
	EXPECT_EQ(exception::THREAD_MODE_PRIORITY, _cpu.current_execution_priority());
	_cpu.interrupt().raise_hardfault();
	_cpu.mem().write32(exception::Type::SVCALL * sizeof(uint32_t), 8192);
	_assembler.seek_to(8192);
	_assembler.emit_nop();
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();
	EXPECT_EQ(exception::HARDFAULT_PRIORITY, _cpu.current_execution_priority());

}

