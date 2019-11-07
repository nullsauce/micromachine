/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "cpu.hpp"
#include "elfio/elfio.hpp"
#include <thread>

namespace  {
	const uint32_t NVIC_ISER = 0xE000E100;
	const uint32_t NVIC_ICER = 0xE000E180;
	const uint32_t NVIC_ISPR = 0xE000E200;
	const uint32_t NVIC_ICPR = 0xE000E280;
	const uint32_t NVIC_IPR0 = 0xE000E400;
	const uint32_t NVIC_IPR1 = 0xE000E404;
	const uint32_t NVIC_IPR2 = 0xE000E408;
	const uint32_t NVIC_IPR3 = 0xE000E40C;
	const uint32_t NVIC_IPR4 = 0xE000E40E;
	const uint32_t NVIC_IPR5 = 0xE000E410;
	const uint32_t NVIC_IPR6 = 0xE000E414;
	const uint32_t NVIC_IPR7 = 0xE000E418;
}

cpu::cpu()
	: _regs(_exception_manager.ctx_switcher)
	, _exception_vector(_nvic, _sphr2_reg, _sphr3_reg)
	, _interrupter(_exception_vector)
	, _generic_io_reg(std::ref(_io_reg_callback))
	, _system_timer(_interrupter)
	, _mem(_interrupter,{
		std::make_pair(0xE000ED1C, std::ref(_sphr2_reg)),
		std::make_pair(0xE000ED20, std::ref(_sphr3_reg)),
		std::make_pair(0xE000E010, std::ref(_system_timer.control_register())),
		std::make_pair(0xE000E014, std::ref(_system_timer.reload_value_register())),
		std::make_pair(0xE000E018, std::ref(_system_timer.current_value_register())),
		std::make_pair(0xE000E01C, std::ref(_system_timer.calib_value_register())),
		std::make_pair(0xE000EF90, std::ref(_generic_io_reg)),
		std::make_pair(NVIC_ISER, std::ref(_nvic._iser_reg)),
		std::make_pair(NVIC_ICER, std::ref(_nvic._icer_reg)),
		std::make_pair(NVIC_ISPR, std::ref(_nvic._ispr_reg)),
		std::make_pair(NVIC_ICPR, std::ref(_nvic._icpr_reg)),
		std::make_pair(NVIC_ICPR, std::ref(_nvic._icpr_reg)),
		std::make_pair(NVIC_IPR0, std::ref(_nvic.priority_reg<0>())),
		std::make_pair(NVIC_IPR1, std::ref(_nvic.priority_reg<1>())),
		std::make_pair(NVIC_IPR2, std::ref(_nvic.priority_reg<2>())),
		std::make_pair(NVIC_IPR3, std::ref(_nvic.priority_reg<3>())),
		std::make_pair(NVIC_IPR4, std::ref(_nvic.priority_reg<4>())),
		std::make_pair(NVIC_IPR5, std::ref(_nvic.priority_reg<5>())),
		std::make_pair(NVIC_IPR6, std::ref(_nvic.priority_reg<6>())),
		std::make_pair(NVIC_IPR7, std::ref(_nvic.priority_reg<7>())),
	})
	, _break_signal(false)
	, _exec_dispatcher(_regs, _mem, _interrupter, _break_signal)
	, _exception_manager(_regs, _mem, _exception_vector)
//	, _interrupt_manager(_mem, _nvic, _sphr2_reg, _sphr3_reg)
	, _initial_pc(0)
	, _debug_instruction_counter(0)
{
#ifdef MICROMACHINE_ENABLE_PRECOND_CHECKS
	fprintf(stderr, "Warning: The CPU is compiled with addtional safety checks that might slow its performance.\n");
#endif
}


bool cpu::load_elf(const std::string &path)
{
	ELFIO::elfio elfReader;
	if (!elfReader.load(path)) {
		return false;
	}

	if (elfReader.get_class() != ELFCLASS32) {
		return false;
	}

	if (elfReader.get_encoding() != ELFDATA2LSB) {
		return false;
	}



	ELFIO::Elf_Half sec_num = elfReader.sections.size();

	uint32_t entryPoint = (uint32_t)elfReader.get_entry();
	//fprintf(stderr, "PC will be set to %08X\n", entryPoint);

	_initial_pc = entryPoint;

	//std::cout << "Number of sections: " << sec_num << std::endl;
	for (int i = 0; i < sec_num; ++i) {
		ELFIO::section *section = elfReader.sections[i];

		if(section->get_flags() & (SHF_ALLOC | SHF_EXECINSTR)) {

			if(0 == section->get_size()) {
				//fprintf(stderr, "Skipping empty section %s\n", section->get_name().c_str());
				continue;
			}

			//fprintf(stderr, "Creating memory section for %s\n", section->get_name().c_str());

			uint8_t* data = (uint8_t*)calloc(1, section->get_size());

			// should we copy data to this section ?
			if(section->get_type() & SHT_PROGBITS) {
				memcpy(data, section->get_data(), section->get_size());
			}

			_mem.map(data, section->get_address(), section->get_size(), section->get_name());
		}
	}

	return true;
}

void cpu::execute(const instruction_pair instr)
{
	_exec_dispatcher.dispatch_instruction(instr);
}

void cpu::reset() {

	uint32_t initial_sp_main = _mem.read32_unchecked(0U) & 0xFFFFFFFC;

	_break_signal = false;
	_exception_vector.reset();
	_exception_manager.reset();
	_regs.reset();
	_regs.app_status_register().reset();
	_regs.set_sp(initial_sp_main);
	_regs.set_pc(_initial_pc);
	_system_timer.reset();
	_sphr2_reg.reset();
	_sphr3_reg.reset();

	// set sp to vector+0
	/*
	uint32_t stack_pointer = _mem.read32(0);
	_regs.set_sp(stack_pointer);
	fprintf(stderr, "reset handler : %08x\n", stack_pointer);

	// branch to entry point
	word reset_handler = _mem.read32(4);
	fprintf(stderr, "reset handler : %08x\n", reset_handler);
	_regs.branch_link_interworking(reset_handler);*/


	/*
	SP_main = MemA[vectortable,4] & 0xFFFFFFFC;
	SP_process = ((bits(30) UNKNOWN):’00’);
	LR = bits(32) UNKNOWN; // Value must be initialised by software
	CurrentMode = Mode_Thread;
	APSR = bits(32) UNKNOWN; // Flags UNPREDICTABLE from reset
	IPSR<5:0> = 0x0; // Exception number clearedat reset
	PRIMASK.PM = '0'; // Priority mask cleared at reset
	CONTROL.SPSEL = '0'; // Current stack is Main
	CONTROL.nPRIV = '0'; // Thread is privileged
	ResetSCSRegs(); // Catch-all function for System Control Space reset
	ExceptionActive[*] = '0'; // All exceptions Inactive
	ClearEventRegister(); // See WFE instruction for more information
	start = MemA[vectortable+4,4]; // Load address of reset routine
	BLXWritePC(start); // Start execution of reset routin

 */
}

instruction_pair cpu::fetch_instruction(uint32_t address) const {
	return instruction_pair(_mem.read32_unchecked(address));
}

// used for debug purposes only
instruction_pair cpu::fetch_instruction_debug(uint32_t address) const {
	uint16_t first_instr = _mem.read16_unchecked(address);
	uint16_t second_instr = _mem.read16_unchecked(address + sizeof(uint16_t)); // always prefetch
	return instruction_pair(first_instr, second_instr);
}



cpu::State cpu::step() {

	if(_break_signal) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		return cpu::State::BREAK;
	}
	_debug_instruction_counter++;

	_system_timer.tick();

	const uint32_t current_addr = _regs.get_pc();
	instruction_pair instr = fetch_instruction(current_addr);
	/*
	fprintf(stderr, "S %08x: %s\n",
		(size_t)current_addr,
		disasm::disassemble_instruction(instr, current_addr).c_str()
	);*/

	if(!_regs.execution_status_register().thumb_bit_set()) {
		// Thumb bit not set
		// all instructions in this state are UNDEFINED .
		_interrupter.raise_hardfault();
	} else {
		_regs.set_pc(current_addr + 4);  // simulate prefetch of 2 instructions
		_regs.reset_pc_dirty_status();
		execute(instr);
	}

	bool hard_fault = false;
	// Next instruction might not be adjacent, if a jump happen.
	uint32_t next_instruction_address = get_next_instruction_address(current_addr, instr);


	ExceptionState* pending_exception = _exception_vector.top_pending();
	if(pending_exception) {
		// The exception entry will handle its own jump/PC settings
		hard_fault = pending_exception->number() == Exception::Type::HARDFAULT;
		_exception_manager.process_pending_exception(current_addr, instr, next_instruction_address);
	} else {
		// Otherwise the PC is restored here
		_regs.set_pc(next_instruction_address);
	}



	// if exception to be serviced
		// context switch
	// else
		// fetch instruction[pc]
		// mimic PC
		// execute
		// if pc didnt change
			// restore pc
			// increment pc



	return hard_fault ? cpu::State::FAULT : cpu::State::RUN;
}

uint32_t cpu::get_next_instruction_address() const {
	const uint32_t instr_addr = _regs.get_pc();
	// TODO: remove unnecessary re-fetch here by storing current instruction
	// in cpu
	instruction_pair instruction = fetch_instruction(instr_addr);
	return get_next_instruction_address(instr_addr, instruction);
}

uint32_t cpu::get_next_instruction_address(uint32_t instr_addr, instruction_pair instruction) const {
	if(!_regs.branch_occured()) {
		return instr_addr + instruction.size();
	} else {
		return _regs.get_pc();
	}
}

const ExceptionStateVector& cpu::exceptions() const {
	return _exception_vector;
}

memory& cpu::mem() {
	return _mem;
}

const memory& cpu::mem() const {
	return _mem;
}

registers& cpu::regs() {
	return _regs;
}

const registers& cpu::regs() const {
	return _regs;
}


