/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "cpu.hpp"
#include "elfio/elfio.hpp"

cpu::cpu()
	: _regs(_exception_manager)
	, _system_timer(_exception_vector)
	, _mem(_exception_vector,{
		std::make_pair(0xE000ED1C, std::ref(_sphr2_reg)),
		std::make_pair(0xE000ED20, std::ref(_sphr3_reg)),
		std::make_pair(0xE000E010, std::ref(_system_timer.control_register())),
		std::make_pair(0xE000E014, std::ref(_system_timer.reload_value_register())),
		std::make_pair(0xE000E018, std::ref(_system_timer.current_value_register())),
		std::make_pair(0xE000E01C, std::ref(_system_timer.calib_value_register())),
	})
	, _break_signal(false)
	, _exec_dispatcher(_regs, _mem, _exception_vector, _break_signal)
	, _exception_manager(_regs, _mem, _exception_vector)
	, _initial_sp(0)
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
		/*
		fprintf(stderr, "%s:\t\t\t %016X %016X %i %i %i %i\n",
			section->get_name().c_str(),
			section->get_address(),
			section->get_size(),
			section->get_flags(),
			section->get_type(),
			section->get_info(),
			section->get_index()
		);*/

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

			if(0 == section->get_name().compare(".stack")) {
				uint32_t sp = section->get_address() + section->get_size() - 1;
				//fprintf(stderr, "SP will be set to %08X\n", sp);
				_initial_sp = sp;
			}
		}
	}

	return true;
}

void cpu::execute(const instruction_pair instr)
{
	_exec_dispatcher.dispatch_instruction(instr);
}

void cpu::reset() {

	_break_signal = false;
	_exception_vector.reset();
	_exception_manager.reset();
	_regs.reset();
	_regs.app_status_register().reset();
	_regs.set_sp(_initial_sp);
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

instruction_pair cpu::fetch_instruction(word address) const {
	bool ok[2] = {false,false};
	halfword first_instr = _mem.read16(address, ok[0]);
	halfword second_instr = _mem.read16(address + sizeof(halfword), ok[1]); // always prefetch
	if(ok[0] && ok[1]) {
		return instruction_pair(first_instr, second_instr);
	} else {
		return instruction_pair(0,0);
	}
}

// used for debug purposes only
instruction_pair cpu::fetch_instruction_debug(word address) const {
	halfword first_instr = _mem.read16_unchecked(address);
	halfword second_instr = _mem.read16_unchecked(address + sizeof(halfword)); // always prefetch
	return instruction_pair(first_instr, second_instr);
}



bool cpu::step() {

	if(_break_signal) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		return true;

	}
	_debug_instruction_counter++;

	_system_timer.tick();

	const word current_addr = _regs.get_pc();
	instruction_pair instr = fetch_instruction(current_addr);

	if(!_regs.execution_status_register().thumb_bit_set()) {
		// Thumb bit not set
		// all instructions in this state are UNDEFINED .
		_exception_vector.raise(exception_number::ex_name::HARDFAULT);
	} else {
		_regs.set_pc(current_addr + 4);  // simulate prefetch of 2 instructions
		_regs.reset_pc_dirty_status();
		execute(instr);
	}

	/*
	fprintf(stderr, "%08x: %s\n",
		(size_t)current_addr,
		disasm::disassemble_instruction(instr, current_addr).c_str()
	);*/

	bool hard_fault = false;
	//bool exception_pending = _exception_manager.prepare_exceptions();
	exception_state* pending_exception = _exception_vector.top_pending_exception();
	if(pending_exception) {
		hard_fault = pending_exception->number() == exception_number(exception_number::ex_name::HARDFAULT);
		_exception_manager.process_pending_exception(current_addr, instr);
	} else if(!_regs.branch_occured()) {
		_regs.set_pc(current_addr + instr.size());
	}

	return hard_fault;
}

const exception_vector& cpu::exceptions() const {
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


