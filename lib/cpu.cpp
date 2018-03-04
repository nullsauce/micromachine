/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "cpu.hpp"
#include "elfio/elfio.hpp"

cpu::cpu()
	: _regs(_active_exceptions[exception::HARDFAULT])
	, _mem(_active_exceptions[exception::HARDFAULT])
	, _exec_dispatcher(_regs, _mem, _active_exceptions)
	, _initial_sp(0)
	, _initial_pc(0)
{}


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
	fprintf(stderr, "PC will be set to %08X\n", entryPoint);

	_initial_pc = entryPoint;

	std::cout << "Number of sections: " << sec_num << std::endl;
	for (int i = 0; i < sec_num; ++i) {
		ELFIO::section *section = elfReader.sections[i];
		fprintf(stderr, "%s:\t\t\t %016X %016X %i %i %i %i\n",
			section->get_name().c_str(),
			section->get_address(),
			section->get_size(),
			section->get_flags(),
			section->get_type(),
			section->get_info(),
			section->get_index()
		);

		if(section->get_flags() & SHF_ALLOC) {


			if(0 == section->get_size()) {
				fprintf(stderr, "Skipping empty section %s\n", section->get_name().c_str());
				continue;
			}

			fprintf(stderr, "Creating memory section for %s\n", section->get_name().c_str());

			uint8_t* data = (uint8_t*)calloc(1, section->get_size());

			// should we copy data to this section ?
			if(section->get_type() & SHT_PROGBITS) {
				memcpy(data, section->get_data(), section->get_size());
			}

			_mem.map(data, section->get_address(), section->get_size(), section->get_name());

			if(0 == section->get_name().compare(".stack")) {
				uint32_t sp = section->get_address() + section->get_size() - 1;
				fprintf(stderr, "SP will be set to %08X\n", sp);
				_initial_sp = sp;
			}
		}
	}



    // Print ELF file segments info
    ELFIO::Elf_Half seg_num = elfReader.segments.size();
    std::cout << "Number of segments: " << seg_num << std::endl;
    for ( int i = 0; i < seg_num; ++i ) {
        const ELFIO::segment* pseg = elfReader.segments[i];
        std::cout << "  [" << i << "] 0x" << std::hex
                  << pseg->get_flags()
                  << "\t0x"
                  << pseg->get_virtual_address()
                  << "\t0x"
                  << pseg->get_file_size()
                  << "\t0x"
                  << pseg->get_memory_size()
                  << std::endl;
        // Access to segments's data
        // const char* p = reader.segments[i]->get_data()
    }
	return true;
}

void cpu::execute(const instruction_pair instr)
{
	_exec_dispatcher.dispatch_instruction(instr);
}

void cpu::reset() {

	_active_exceptions.clear();
	enter_thread_mode();
	_regs.reset();
	_regs.app_status_register().reset();
	_regs.set_sp(_initial_sp);
	_regs.set_pc(_initial_pc);

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

	if(!_regs.execution_status_register().thumb_bit_set()) {
		// Thumb bit not set
		// all instructions in this state are UNDEFINED .
		signal_exception(exception::HARDFAULT);
	}
	if(_active_exceptions.any_signaled()) {
		fprintf(stderr, "Exception signaled\n");
		return true;
	} else {

		const word current_addr = _regs.get_pc();
		instruction_pair instr = fetch_instruction(current_addr);

		_regs.set_pc(current_addr + 4);  // simulate prefetch of 2 instructions
		_regs.reset_pc_dirty_status();

		execute(instr);

		fprintf(stderr, "%08x: %s\n",
			(size_t)current_addr,
			disasm::disassemble_instruction(instr, current_addr).c_str()
		);

		bool hard_fault = active_exceptions().is_signaled(exception::HARDFAULT);
		bool fault = hard_fault;
		if(!_regs.branch_occured()) {
			if(hard_fault) {
				_regs.set_pc(current_addr);
			} else {
				_regs.set_pc(current_addr + instr.size());
			}
		}

		if(active_exceptions().any_signaled()) {
			//exception_entry(active_exceptions.storage());
		}

		return hard_fault;
	}
}

const exception_vector& cpu::active_exceptions() const {
	return _active_exceptions;
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


void cpu::push_stack() {

	const size_t stack_frame_size = 32;
	const uint32_t sp_mask = ~((uint32_t)0b100);
	const bool frame_ptr_align = _regs.get_sp().bit(2);

	uint32_t frame_ptr = (_regs.get_sp() - stack_frame_size) & sp_mask;
	_regs.set_sp(frame_ptr);

	// TODO: ReturnAddress()
	uint32_t return_address = regs().get_pc() - 2;

	uint xpsr_status = 	_regs.xpsr_register().uint(0, 8) |
						(frame_ptr_align << 8) |
						_regs.xpsr_register().uint(10, 22) << 10;

	_mem.write32(frame_ptr+0,  _regs.get(0));
	_mem.write32(frame_ptr+4,  _regs.get(1));
	_mem.write32(frame_ptr+8,  _regs.get(2));
	_mem.write32(frame_ptr+12, _regs.get(3));
	_mem.write32(frame_ptr+16, _regs.get(12));
	_mem.write32(frame_ptr+20, _regs.get_lr());
	_mem.write32(frame_ptr+24, return_address);
	_mem.write32(frame_ptr+28, xpsr_status);


	if(_regs.exec_mode_register().is_handler_mode()) {
		_regs.set_lr(0xFFFFFFF1);
	} else if(_regs.control_register().sp_sel()) {
		_regs.set_lr(0xFFFFFFF9);
	} else {
		_regs.set_lr(0xFFFFFFFD);
	}
}

void cpu::signal_exception(exception ex)  {
	precond(exception::INVALID != ex, "Bad exception identifier");
	_active_exceptions[ex] = true;
}

void cpu::exception_entry(exception ex) {
	push_stack();
	take_exception(ex);
}

void cpu::enter_handler_mode() {
	_regs.exec_mode_register().set_handler_mode();
}

void cpu::enter_thread_mode() {
	_regs.exec_mode_register().set_thread_mode();
}

bool cpu::is_priviledged_mode() const {
	return _regs.exec_mode_register().is_handler_mode() &&
		!_regs.control_register().n_priv();
}

void cpu::take_exception(exception exception) {
	// enter handler mode
	enter_handler_mode();

	// set ipsr with exception number
	_regs.interrupt_status_register().set_exception(exception);

	// stack is now SP_main
	_regs.control_register().set_sp_sel(0);

	_active_exceptions[exception] = true;

	//SCS_UpdateStatusRegs();
	//SetEventRegister();
	//InstructionSynchronizationBarrier();
	uint32_t handler_address = _mem.read32(4*(size_t)exception);
	_regs.branch_link_interworking(handler_address);
}

