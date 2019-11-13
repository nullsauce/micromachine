

#include "CpuTestHarness.hpp"
#include "mini_assembler.hpp"

class ExceptionTestHarness : public CpuTestHarness {
	protected:
		mini_assembler _assembler;

		ExceptionTestHarness()
		: _assembler(_code_gen)
		{}

		void install_handler(exception::Type ex, uint32_t address) {
			_cpu.mem().write32(ex * sizeof(uint32_t), address);
		}
};


TEST_F(ExceptionTestHarness, DefaultPriorityIsThreadModeAtStartup)
{
	EXPECT_EQ(exception::THREAD_MODE_PRIORITY, _cpu.current_execution_priority());
}



TEST_F(ExceptionTestHarness, ExecutionPriorityChangesWhenExceptionIsTaken)
{
	EXPECT_EQ(exception::THREAD_MODE_PRIORITY, _cpu.current_execution_priority());
	_cpu.interrupt().raise_hardfault();
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();
	EXPECT_EQ(exception::HARDFAULT_PRIORITY, _cpu.current_execution_priority());
}
/*
TEST_F(ExceptionTestHarness, ExecutionPriorityGoesBackToDefaultAfterExceptionIsTaken)
{
	EXPECT_EQ(exception::THREAD_MODE_PRIORITY, _cpu.current_execution_priority());
	_cpu.interrupt().raise_hardfault();
	install_handler(exception::Type::HARDFAULT, 0x2000 | 1);
	_assembler
	.seek_to(0x2000)
	.emit_push_lr()
	.emit_pop_pc();

	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	setExpectedRegisterValue(PC, 0x2000);
	setExpectedStackGrowthSinceBeginning(32);
	step();
	EXPECT_EQ(exception::HARDFAULT_PRIORITY, _cpu.current_execution_priority());

	setExpectedStackGrowthSinceBeginning(36);
	setExpectedRegisterValue(PC, 0x2002);
	step();

	setExpectedRegisterValue(PC, INITIAL_PC);
	setExpectedRegisterValue(LR, RETURN_TO_THREAD_FROM_SP_MAIN);
	setExpectedStackGrowthSinceBeginning(0);
	step();
	EXPECT_EQ(exception::THREAD_MODE_PRIORITY, _cpu.current_execution_priority());
}*/

