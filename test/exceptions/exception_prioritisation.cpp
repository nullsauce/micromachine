

#include "CpuTestHarness.hpp"
#include "exception/exception_defs.hpp"
#include "mini_assembler.hpp"

class ExceptionTestHarness : public CpuTestHarness {
	protected:
		mini_assembler _assembler;

		ExceptionTestHarness()
		: _assembler(_code_gen)
		{}

		void install_handler(micromachine::system::exception::Type ex, uint32_t address) {
			_mcu.get_cpu().mem().write32(ex * sizeof(uint32_t), address);
		}
};


TEST_F(ExceptionTestHarness, DefaultPriorityIsThreadModeAtStartup)
{
	EXPECT_EQ(micromachine::system::exception::THREAD_MODE_PRIORITY,
			  _mcu.get_cpu().current_execution_priority());
}



TEST_F(ExceptionTestHarness, ExecutionPriorityChangesWhenExceptionIsTaken)
{
	EXPECT_EQ(micromachine::system::exception::THREAD_MODE_PRIORITY,
			  _mcu.get_cpu().current_execution_priority());
	_mcu.get_cpu().interrupt().raise_hardfault();
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();
	EXPECT_EQ(micromachine::system::exception::HARDFAULT_PRIORITY,
			  _mcu.get_cpu().current_execution_priority());
}

TEST_F(ExceptionTestHarness, ExceptionPreAndPostState)
{
	// initially in thread mode
	EXPECT_EQ(micromachine::system::exception::THREAD_MODE_PRIORITY,
			  _mcu.get_cpu().current_execution_priority());

	// we are creating a fake exception handler at this address
	const uint32_t exception_handler_stub = 0x2000;

	// hardfault handler points to this address now
	install_handler(micromachine::system::exception::Type::HARDFAULT, exception_handler_stub | 1);

	// write a simple empty function in the hardfault handler.
	// the function simply returns using BX LR
	_assembler.seek_to(exception_handler_stub).emit_bx_lr();

	// raise hardfault
	_mcu.get_cpu().interrupt().raise_hardfault();

	// expect the cpu to have branched into the
	// handler at the next step by checking registers and stack growth.
	// all the registers are pushed to the stack on exception entry, hence the 32.
	setExpectedExceptionTaken(CPU_STEP_HARDFAULT);
	step();

	// check that the priority is set to hard fault priority
	EXPECT_EQ(micromachine::system::exception::HARDFAULT_PRIORITY,
			  _mcu.get_cpu().current_execution_priority());

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
	EXPECT_EQ(micromachine::system::exception::THREAD_MODE_PRIORITY,
			  _mcu.get_cpu().current_execution_priority());
}

