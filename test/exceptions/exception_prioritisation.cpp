

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

TEST_F(ExceptionTestHarness, ExecutionPriorityGoesBackToDefaultAfterExceptionIsTaken)
{
	// intitially in thread mode
	EXPECT_EQ(exception::THREAD_MODE_PRIORITY, _cpu.current_execution_priority());

	const uint32_t exception_handler_stub = 0x2000;

	// handle hardfaults at 0x2000 with a stub
	install_handler(exception::Type::HARDFAULT, exception_handler_stub | 1);

	_assembler.seek_to(exception_handler_stub).emit_bx_lr();

	// raise hardfault
	_cpu.interrupt().raise_hardfault();

	// expect the cpu to have branched into the
	// handler at the next step by checking registers and stack growth.
	// all the registers are pushed to the stack on exception entry, hence the 32.
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();

	// check that the priority is set to hard fault priority
	EXPECT_EQ(exception::HARDFAULT_PRIORITY, _cpu.current_execution_priority());

	// Check that the BX LR instruction was executed properly.
	// PC should be back to its initial value
	setExpectedRegisterValue(registers::PC, INITIAL_PC);
	// LR should be back to its initial value
	setExpectedRegisterValue(registers::LR, INITIAL_LR);
	// Stack should be folded back to zero
	setExpectedStackGrowthSinceBeginning(0);
	// No exception should be running
	setExpectNoExceptionTaken();
	step();
	EXPECT_EQ(exception::THREAD_MODE_PRIORITY, _cpu.current_execution_priority());
}

