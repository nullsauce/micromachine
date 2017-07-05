//
// Created by fla on 17-5-9.
//

#ifndef MICROMACHINE_CPU_HPP
#define MICROMACHINE_CPU_HPP

#include "types.hpp"
#include "registers.hpp"
#include "apsr_register.hpp"
#include "exec.hpp"
#include "instructions.hpp"
#include "exec.hpp"
#include "memory.hpp"
#include "exec_mode.hpp"
#include "exception.hpp"
#include "exception_vector.hpp"

static bool is_siadsumoco(const halfword& instruction) {
	return 0 == instruction.uint(14, 2);
}

static bool is_data_processing(const halfword& instruction) {
	return 0b010000 == instruction.uint(10, 6);
}

static bool is_sdibe(const halfword& instruction) {
	return 0b010001 == instruction.uint(10, 6);
}

static bool is_lsl_imm(const halfword& instruction) {
	return is_siadsumoco(instruction) &&
		   0b000 == instruction.uint(11, 3);

}

static bool is_lsr_imm(const halfword& instruction) {
	return is_siadsumoco(instruction) &&
		   0b001 == instruction.uint(11, 3);
}

static bool is_asr_imm(const halfword& instruction) {
	return is_siadsumoco(instruction) &&
		   0b010 == instruction.uint(11, 3);
}

static bool is_add_reg(const halfword& instruction) {
	return is_siadsumoco(instruction) &&
		   0b01100 == instruction.uint(9, 5);
}

static bool is_subs_reg(const halfword& instruction) {
	return is_siadsumoco(instruction) &&
		   0b01101 == instruction.uint(9, 5);
}

static bool is_add_imm(const halfword& instruction) {
	return is_siadsumoco(instruction) &&
		   0b01110 == instruction.uint(9, 5);
}

static bool is_subs_imm(const halfword& instruction) {
	return is_siadsumoco(instruction) &&
		   0b01111 == instruction.uint(9, 5);
}

static bool is_mov_imm(const halfword& instruction) {
	return is_siadsumoco(instruction) &&
		   0b100 == instruction.uint(11, 3);
}

static bool is_cmp_imm(const halfword& instruction) {
	return is_siadsumoco(instruction) &&
		   0b101 == instruction.uint(11, 3);
}

static bool is_add_imm_t2(const halfword& instruction) {
	return is_siadsumoco(instruction) &&
		   0b110 == instruction.uint(11, 3);
}

static bool is_subs_imm8(const halfword& instruction) {
	return is_siadsumoco(instruction) &&
		   0b111 == instruction.uint(11, 3);
}


static bool is_and_reg(const halfword& instruction) {
	return is_data_processing(instruction) &&
		   0b0000 == instruction.uint(6, 4);
}

static bool is_xor_reg(const halfword& instruction) {
	return is_data_processing(instruction) &&
		   0b0001 == instruction.uint(6, 4);
}

static bool is_lsl_reg(const halfword& instruction) {
	return is_data_processing(instruction) &&
		   0b0010 == instruction.uint(6, 4);
}

static bool is_lsr_reg(const halfword& instruction) {
	return is_data_processing(instruction) &&
		   0b0011 == instruction.uint(6, 4);
}

static bool is_asr_reg(const halfword& instruction) {
	return is_data_processing(instruction) &&
		   0b0100 == instruction.uint(6, 4);
}

static bool is_adc_reg(const halfword& instruction) {
	return is_data_processing(instruction) &&
		   0b0101 == instruction.uint(6, 4);
}

static bool is_sub_c_reg(const halfword& instruction) {
	return is_data_processing(instruction) &&
		   0b0110 == instruction.uint(6, 4);
}

static bool is_ror_reg(const halfword& instruction) {
	return is_data_processing(instruction) &&
		   0b0111 == instruction.uint(6, 4);
}

static bool is_tst_reg(const halfword& instruction) {
	return is_data_processing(instruction) &&
		   0b1000 == instruction.uint(6, 4);
}

static bool is_rsb_imm(const halfword& instruction) {
	return is_data_processing(instruction) &&
		   0b1001 == instruction.uint(6, 4);
}

static bool is_cmn_reg(const halfword& instruction) {
	return is_data_processing(instruction) &&
		   0b1011 == instruction.uint(6, 4);
}

static bool is_cmp_reg(const halfword& instruction) {
	return is_data_processing(instruction) &&
		   0b1010 == instruction.uint(6, 4);
}

static bool is_lor_reg(const halfword& instruction) {
	return is_data_processing(instruction) &&
		   0b1100 == instruction.uint(6, 4);
}

static bool is_mul_reg(const halfword& instruction) {
	return is_data_processing(instruction) &&
		   0b1101 == instruction.uint(6, 4);
}

static bool is_bitclear_reg(const halfword& instruction) {
	return is_data_processing(instruction) &&
		   0b1110 == instruction.uint(6, 4);
}

static bool is_not_reg(const halfword& instruction) {
	return is_data_processing(instruction) &&
		   0b1111 == instruction.uint(6, 4);
}


static bool is_add_highreg(const halfword& instruction) {
	return is_sdibe(instruction) &&
		   0b00 == instruction.uint(8, 2);
}

static bool is_sdibe_unpredictable_0(const halfword& instruction) {
	return is_sdibe(instruction) &&
		   0b0100 == instruction.uint(6, 4);
}

static bool is_cmp_highreg(const halfword& instruction) {
	return is_sdibe(instruction) && (
		(0b0101 == instruction.uint(6, 4)) ||
		(0b011 == instruction.uint(7, 3))
	);
}

static bool is_mov_highreg(const halfword& instruction) {
	return is_sdibe(instruction) &&
		   0b10 == instruction.uint(8, 2);
}

static bool is_bx(const halfword& instruction) {
	return is_sdibe(instruction) &&
		   0b110 == instruction.uint(7, 3);
}

static bool is_blx(const halfword& instruction) {
	return is_sdibe(instruction) &&
		   0b111 == instruction.uint(7, 3);
}

static bool is_ldr_literal(const halfword& instruction) {
	return 0b01001 == instruction.uint(11, 5);
}

static bool is_store_reg_word_reg(const halfword& instruction) {
	return 0b0101000 == instruction.uint(9, 7);
}

static bool is_store_reg_halfword_reg(const halfword& instruction) {
	return 0b0101001 == instruction.uint(9, 7);
}

static bool is_store_reg_byte_reg(const halfword& instruction) {
	return 0b0101010 == instruction.uint(9, 7);
}

static bool is_load_reg_sbyte_reg(const halfword& instruction) {
	return 0b0101011 == instruction.uint(9, 7);
}

static bool is_ldr_reg(const halfword& instruction) {
	return 0b0101100 == instruction.uint(9, 7);
}

static bool is_load_reg_halfword_reg(const halfword& instruction) {
	return 0b0101101 == instruction.uint(9, 7);
}

static bool is_load_reg_byte_reg(const halfword& instruction) {
	return 0b0101110 == instruction.uint(9, 7);
}

static bool is_load_reg_shalfword_reg(const halfword& instruction) {
	return 0b0101111 == instruction.uint(9, 7);
}

static bool is_store_word_imm(const halfword& instruction) {
	return 0b01100 == instruction.uint(11, 5);
}

static bool is_ldr_imm(const halfword& instruction) {
	return 0b01101 == instruction.uint(11, 5);
}

static bool is_store_byte_imm(const halfword& instruction) {
	return 0b01110 == instruction.uint(11, 5);
}

static bool is_load_byte_imm(const halfword& instruction) {
	return 0b01111 == instruction.uint(11, 5);
}

static bool is_store_halfword_imm(const halfword& instruction) {
	return 0b10000 == instruction.uint(11, 5);
}

static bool is_load_halfword_imm(const halfword& instruction) {
	return 0b10001 == instruction.uint(11, 5);
}

static bool is_store_word_sp_imm(const halfword& instruction) {
	return 0b10010 == instruction.uint(11, 5);
}

static bool is_ldr_imm_sp(const halfword& instruction) {
	return 0b10011 == instruction.uint(11, 5);
}

static bool is_adr(const halfword& instruction) {
	return 0b10100 == instruction.uint(11, 5);
}

static bool is_add_sp_imm(const halfword& instruction) {
	return 0b10101 == instruction.uint(11, 5);
}

static bool is_add_sp_imm_t2(const halfword& instruction) {
	return 0b101100000 == instruction.uint(7, 9);
}

static bool is_sub_sp_imm(const halfword& instruction) {
	return 0b101100001 == instruction.uint(7, 9);
}

static bool is_sxth(const halfword& instruction) {
	return 0b1011001000 == instruction.uint(6, 10);
}

static bool is_sxtb(const halfword& instruction) {
	return 0b1011001001 == instruction.uint(6, 10);
}

static bool is_uxth(const halfword& instruction) {
	return 0b1011001010 == instruction.uint(6, 10);
}

static bool is_uxtb(const halfword& instruction) {
	return 0b1011001011 == instruction.uint(6, 10);
}

static bool is_push(const halfword& instruction) {
	return 0b1011010 == instruction.uint(9, 7);
}

static bool is_cps(const halfword& instruction) {
	return 0b10110110011 == instruction.uint(5, 11);
}

static bool is_rev_word(const halfword& instruction) {
	return 0b1011101000 == instruction.uint(6, 10);
}

static bool is_rev_packed_halfword(const halfword& instruction) {
	return 0b1011101001 == instruction.uint(6, 10);
}

static bool is_rev_signed_halfword(const halfword& instruction) {
	return 0b1011101011 == instruction.uint(6, 10);
}

static bool is_pop(const halfword& instruction) {
	return 0b1011110 == instruction.uint(9, 7);
}

static bool is_breakpoint(const halfword& instruction) {
	return 0b10111110 == instruction.uint(8, 8);
}

static bool is_hints(const halfword& instruction) {
	return 0b10111111 == instruction.uint(8, 8);
}

static bool is_nop(const halfword& instruction) {
	return 0b1011111100000000 == instruction;
}

static bool is_stm(const halfword& instruction) {
	return 0b11000 == instruction.uint(11, 5);
}

static bool is_ldm(const halfword& instruction) {
	return 0b11001 == instruction.uint(11, 5);
}

static bool is_branch(const halfword& instruction) {
	return 0b1101 == instruction.uint(12, 4) &&
		   (instruction.uint(9, 3) != 0b111);
}

static bool is_svc(const halfword& instruction) {
	return 0b1101 == instruction.uint(12, 4) &&
		   0b1111 == instruction.uint(8, 4);
}


static bool is_unconditional_branch(const halfword& instruction) {
	return 0b11100 == instruction.uint(11, 5);
}

static bool is_32bit_thumb_encoding(const halfword& instruction) {
	return 0b111 == instruction.uint(13, 3) &&
		   0b00 !=  instruction.uint(11, 2);
}


static bool is_32bit_thumb_br_misc_ctl(const halfword& first, const halfword& second) {
	return is_32bit_thumb_encoding(first) &&
		   0b10 == first.uint(11, 2) &&
		   0b1  == second.uint(15, 1);
}

static bool is_32bit_thumb_msr(const halfword& first, const halfword& second) {
	return is_32bit_thumb_br_misc_ctl(first, second) &&
		   0b011100 == first.uint(5, 6) &&
		   0b000 	== (second.uint(12, 3) & 0b101);
}

static bool is_32bit_thumb_misc_ctl(const halfword& first, const halfword& second) {
	return is_32bit_thumb_br_misc_ctl(first, second) &&
		   0b0111011 == first.uint(4, 7) &&
		   0b000 	== (second.uint(12, 3) & 0b101);
}

static bool is_32bit_thumb_mrs(const halfword& first, const halfword& second) {
	return is_32bit_thumb_br_misc_ctl(first, second) &&
		   0b011111 == first.uint(5, 6) &&
		   0b000 	== (second.uint(12, 3) & 0b101);
}

static bool is_32bit_thumb_bl(const halfword& first, const halfword& second) {
	return is_32bit_thumb_br_misc_ctl(first, second) &&
		   0b101 	== (second.uint(12, 3) & 0b101);
}

static bool is_undefined(const halfword& instr) {
	return 0b11011110 == instr.uint(8, 8);
}

static bool is_undefined32(const halfword& first, const halfword& second) {
	return is_32bit_thumb_encoding(first) &&
			0b111101111111 == first.uint(4, 12) &&
			0b1010 == second.uint(12, 4);
}

class cpu {



public:


	void dispatch_and_exec(halfword instr) {
		cpu::dispatch_and_exec(instr, _regs, _status_reg, _mem, _active_exceptions);
	}

	static
	void dispatch_and_exec(halfword instr, registers& regs, apsr_register& status_reg, memory& mem, exception_vector exceptions) {

		if(is_nop(instr)) return;

		//fprintf(stderr, "%s\n", instr.to_string().c_str());

		// Shift (immediate), add, subtract, move, and compare
		if(is_lsl_imm(instr)) {
			if(0 == instr.uint(9,5) && 0 == instr.uint(6, 2)) {
				// When opcode is 0b00000 , and bits[8:6] are 0b000 , this encoding is MOV reg
				//TODO: cps

				exec(movs(instr), regs, status_reg);

			}
			exec(lsl_imm(instr), regs, status_reg);
		} else if(is_lsr_imm(instr)) {
			exec(lsr_imm(instr), regs, status_reg);
		} else if(is_asr_imm(instr)) {
			exec(asr_imm(instr), regs, status_reg);
		} else if(is_add_reg(instr)) {
			exec(add_reg(instr), regs, status_reg);
		} else if(is_subs_reg(instr)) {
			exec(subs_reg(instr), regs, status_reg);
		} else if(is_add_imm(instr)) {
			exec(add_imm(instr), regs, status_reg);
		} else if(is_subs_imm(instr)) {
			exec(subs_imm(instr), regs, status_reg);
		} else if(is_mov_imm(instr)) {
			exec(mov_imm(instr), regs, status_reg);
		} else if(is_cmp_imm(instr)) {
			exec(cmp_imm(instr), regs, status_reg);
		} else if(is_add_imm_t2(instr)) {
			exec(add_imm_t2(instr), regs, status_reg);
		} else if(is_subs_imm8(instr)) {
			exec(subs_imm8(instr), regs, status_reg);



			// Data processing
		} else if(is_and_reg(instr)) {
			exec(and_reg(instr), regs, status_reg);
		} else if(is_xor_reg(instr)) {
			exec(eor_reg(instr), regs, status_reg);
		} else if(is_lsl_reg(instr)) {
			exec(lsl_reg(instr), regs, status_reg);
		} else if(is_lsr_reg(instr)) {
			exec(lsr_reg(instr), regs, status_reg);
		} else if(is_asr_reg(instr)) {
			exec(asr_reg(instr), regs, status_reg);
		} else if(is_adc_reg(instr)) {
			exec(adc(instr), regs, status_reg);
		} else if(is_sub_c_reg(instr)) {
			exec(sub_c_reg(instr), regs, status_reg);
		} else if(is_ror_reg(instr)) {
			exec(ror_reg(instr), regs, status_reg);
		} else if(is_tst_reg(instr)) {
			exec(tst_reg(instr), regs, status_reg);
		} else if(is_rsb_imm(instr)) {
			exec(rsb_imm(instr), regs, status_reg);
		} else if(is_cmp_reg(instr)) {
			exec(cmp_reg(instr), regs, status_reg);
		} else if(is_cmn_reg(instr)) {
			exec(cmn_reg(instr), regs, status_reg);
		} else if(is_lor_reg(instr)) {
			exec(lor_reg(instr), regs, status_reg);
		} else if(is_mul_reg(instr)) {
			exec(mul_reg(instr), regs, status_reg);
		} else if(is_bitclear_reg(instr)) {
			exec(bic_reg(instr), regs, status_reg);
		} else if(is_not_reg(instr)) {
			exec(not_reg(instr), regs, status_reg);


			// Special data instructions and branch and exchange
		} else if(is_add_highreg(instr)) {
			exec(add_highreg(instr), regs, status_reg);
		} else if(is_sdibe_unpredictable_0(instr)) {
			// fault !
		} else if(is_cmp_highreg(instr)) {
			exec(cmp_highreg(instr), regs, status_reg);
		} else if(is_mov_highreg(instr)) {
			exec(mov_highreg(instr), regs, status_reg);
		} else if(is_bx(instr)) {
			exec(bx(instr), regs, status_reg);
		} else if(is_blx(instr)) {
			exec(blx(instr), regs, status_reg);
		} else if(is_ldr_literal(instr)) {
			exec(ldr_literal(instr), regs, status_reg, mem);


			// load store
		} else if(is_store_reg_word_reg(instr)) {
			exec(store_reg_word_reg(instr), regs, status_reg, mem);
		} else if(is_store_reg_halfword_reg(instr)) {
			exec(store_reg_halfword_reg(instr), regs, status_reg, mem);
		} else if(is_store_reg_byte_reg(instr)) {
			exec(store_reg_byte_reg(instr), regs, status_reg, mem);
		} else if(is_load_reg_sbyte_reg(instr)) {
			exec(load_reg_sbyte_reg(instr), regs, status_reg, mem);
		} else if(is_ldr_reg(instr)) {
			exec(ldr_reg(instr), regs, status_reg, mem);
		} else if(is_load_reg_halfword_reg(instr)) {
			exec(load_reg_halfword_reg(instr), regs, status_reg, mem);
		} else if(is_load_reg_byte_reg(instr)) {
			exec(load_reg_byte_reg(instr), regs, status_reg, mem);
		} else if(is_load_reg_shalfword_reg(instr)) {
			exec(load_reg_shalfword_reg(instr), regs, status_reg, mem);
		} else if(is_store_word_imm(instr)) {
			exec(store_word_imm(instr), regs, status_reg, mem);
		} else if(is_ldr_imm(instr)) {
			exec(ldr_imm(instr), regs, status_reg, mem);
		} else if(is_store_byte_imm(instr)) {
			exec(store_byte_imm(instr), regs, status_reg, mem);
		} else if(is_load_byte_imm(instr)) {
			exec(load_byte_imm(instr), regs, status_reg, mem);
		} else if(is_store_halfword_imm(instr)) {
			exec(store_halfword_imm(instr), regs, status_reg, mem);
		} else if(is_load_halfword_imm(instr)) {
			exec(load_halfword_imm(instr), regs, status_reg, mem);
		} else if(is_store_word_sp_imm(instr)) {
			exec(store_word_sp_imm(instr), regs, status_reg, mem);
		} else if(is_ldr_imm_sp(instr)) {
			exec(ldr_imm_sp(instr), regs, status_reg, mem);


			// PC relative address
		} else if(is_adr(instr)) {
			exec(adr(instr), regs);

			// Generate SP-relative address: ADD (SP plus immediate)
		} else if(is_add_sp_imm(instr)) {
			exec(add_sp_imm(instr), regs);


			// Miscellaneous 16-bit instructions on page A5-8
		} else if(is_add_sp_imm_t2(instr)) {
			exec(add_sp_imm_t2(instr), regs);
		} else if(is_sub_sp_imm(instr)) {
			exec(sub_sp_imm(instr), regs);
		} else if(is_sxth(instr)) {
			exec(sxth(instr), regs);
		} else if(is_sxtb(instr)) {
			exec(sxtb(instr), regs);
		} else if(is_uxth(instr)) {
			exec(uxth(instr), regs);
		} else if(is_uxtb(instr)) {
			exec(uxtb(instr), regs);
		} else if(is_push(instr)) {
			exec(push(instr), regs, mem);
		} else if(is_cps(instr)) {
			//TODO: cps
			precond_fail("unimplemented");
		} else if(is_rev_word(instr)) {
			exec(rev_word(instr), regs);
		} else if(is_rev_packed_halfword(instr)) {
			exec(rev_packed_halfword(instr), regs);
		} else if(is_rev_signed_halfword(instr)) {
			exec(rev_packed_signed_halfword(instr), regs);
		} else if(is_pop(instr)) {
			exec(pop(instr), regs, mem);
		} else if(is_breakpoint(instr)) {
			// TODO:
			precond_fail("unimplemented");
		} else if(is_hints(instr)) {
			// TODO:
			precond_fail("unimplemented");


			// Store multiple registers, see STM, STMIA, STMEA on page A6-175
		} else if(is_stm(instr)) {
			exec(stm(instr), regs, mem);

			// Load multiple registers, see LDM, LDMIA, LDMFD on page A6-137
		} else if(is_ldm(instr)) {
			exec(ldm(instr), regs, mem);


		} else if(is_branch(instr)) {
			fprintf(stderr, "%s\n", branch(instr).to_string().c_str());
			exec(branch(instr), regs, status_reg);
		} else if(is_svc(instr)) {
			exec(svc(instr));
		} else if(is_unconditional_branch(instr)) {
			exec(unconditional_branch(instr), regs, status_reg);


		// 32bit encodings
		} else if(is_32bit_thumb_encoding(instr)) {

			// fetch next instruction
			halfword second_instruction = mem.read16(regs.get_pc() - 2);

			// TODO: check if necessary
			// advance pc to keep the 4 bytes ahead
			// regs.set_pc(regs.get_pc()+2);

			if(is_undefined32(instr, second_instruction)) {
				exceptions[exception::HARDFAULT] = true;
			} else if(is_32bit_thumb_br_misc_ctl(instr, second_instruction)) {

				if(is_32bit_thumb_msr(instr, second_instruction)) {
					precond_fail("unimplemented msr");
				} else if(is_32bit_thumb_misc_ctl(instr, second_instruction)) {
					precond_fail("unimplemented 32 bit misc ctl intructions");
				} else if(is_32bit_thumb_mrs(instr, second_instruction)) {
					precond_fail("unimplemented mrs");
				} else if(is_32bit_thumb_bl(instr, second_instruction)) {
					exec(bl_imm(instr, second_instruction), regs);
				} else {
					precond_fail("unimplemented 32bit misc br and ctrl instruction")
				}


			} else {
				precond_fail("unimplemented 32bit instruction");
			}

		} else if(is_undefined(instr)) {
			exceptions[exception::HARDFAULT] = true;
		} else {
			fprintf(stderr, "unhanlded instruction %04X\n", (uint32_t)instr);
			precond_fail("unimplemented");
		}

	}



	cpu()
		: _exec_mode(exec_mode::thread)
		, _regs(_exec_mode, _active_exceptions[exception::HARDFAULT])
		, _mem(_active_exceptions[exception::HARDFAULT]){

		reset();
	}

	void reset() {
		_active_exceptions.clear();
		_exec_mode = exec_mode::thread;
		_regs.reset();
		_status_reg = 0;
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


	void step() {

		if(_active_exceptions.any_signaled()) {
			fprintf(stderr, "Exception signaled\n");
		} else {


			word current_instr = _regs.get_pc();
			halfword instr = _mem.read16(current_instr);
			fprintf(stderr, "exec %08x %d %s\n",
					(uint32_t) current_instr,
					(uint32_t) current_instr,
					instr.to_string().c_str()
			);
			_regs.set_pc(current_instr + 4); // prefetch 2 instructions
			_regs.reset_pc_dirty_status();
			dispatch_and_exec(instr);

			// TODO: how does this behaves with exceptions
			if(active_exceptions().is_signaled(exception::HARDFAULT)) {
				_regs.set_pc(current_instr);
			} else if (!_regs.branch_occured()) {
				_regs.set_pc(current_instr + 2);
			}
		}
	}

	const exception_vector& active_exceptions() const {
		return _active_exceptions;
	}

	memory& mem() {
		return _mem;
	}

	const memory& mem() const {
		return _mem;
	}

	registers& regs() {
		return _regs;
	}

	const registers& regs() const {
		return _regs;
	}

	// for testing
	apsr_register& xpsr() {
		return _status_reg;
	}

	apsr_register& flags() {
		return _status_reg;
	}

private:

	bool is_handler_mode() const {
		return exec_mode::handler == _exec_mode;
	}

	bool is_thread_mode() const {
		return exec_mode::thread == _exec_mode;
	}

	bool is_priviledged_mode() const {
		return is_handler_mode() &&
			!_regs.control_register().n_priv();
	}

	void push_stack() {

		const size_t stack_frame_size = 32;
		const uint32_t sp_mask = ~((uint32_t)0b100);
		const bool frame_ptr_align = _regs.get_sp().bit(2);

		uint32_t frame_ptr = (_regs.get_sp() - stack_frame_size) & sp_mask;
		_regs.set_sp(frame_ptr);

		// TODO: ReturnAddress()
		uint32_t return_address = regs().get_pc() - 2;

		uint xpsr_status = 	_status_reg.uint(0, 8) |
							(frame_ptr_align << 8) |
							_status_reg.uint(10, 22) << 10;

		_mem.write32(frame_ptr+0,  _regs.get(0));
		_mem.write32(frame_ptr+4,  _regs.get(1));
		_mem.write32(frame_ptr+8,  _regs.get(2));
		_mem.write32(frame_ptr+12, _regs.get(3));
		_mem.write32(frame_ptr+16, _regs.get(12));
		_mem.write32(frame_ptr+20, _regs.get_lr());
		_mem.write32(frame_ptr+24, return_address);
		_mem.write32(frame_ptr+28, xpsr_status);


		if(is_handler_mode()) {
			_regs.set_lr(0xFFFFFFF1);
		} else if(_regs.control_register().sp_sel()) {
			_regs.set_lr(0xFFFFFFF9);
		} else {
			_regs.set_lr(0xFFFFFFFD);
		}
	}

	void signal_exception(exception ex)  {
		precond(exception::INVALID != ex, "Bad exception identifier");
		_active_exceptions[ex] = true;
	}



	void take_exception(exception exception) {
		// enter handler mode
		_exec_mode = exec_mode::handler;

		// set ipsr with exception number
		_status_reg = (_status_reg & ~binops::make_mask<uint32_t>(5))
						| (size_t)exception;

		// stack is now SP_main
		_regs.control_register().set_sp_sel(0);

		_active_exceptions[exception] = true;

		// TODO: SCS_UpdateStatusRegs()
		// TODO: SetEventRegister()
		// TODO: InstructionSynchronizationBarrier()


		static const uint32_t vector_table = 4;
		uint32_t handler_address = _mem.read32(vector_table + ((size_t)exception*4));

		_regs.branch(handler_address);
	}

	exception_vector	_active_exceptions;
	registers 			_regs;
	apsr_register 		_status_reg;
	memory 				_mem;
	exec_mode 			_exec_mode;


};

#endif //MICROMACHINE_CPU_HPP
