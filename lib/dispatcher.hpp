/* Copyright (C) The Micromachine project - 2016 - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of The Micromachine project.

*/

#ifndef MICROMACHINE_DISPATCH_HPP
#define MICROMACHINE_DISPATCH_HPP

#include "instructions.hpp"

namespace {

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
}

#include "exception_vector.hpp"

class dispatcher {

private:
	exception_vector& exceptions;

public:

	dispatcher(exception_vector& _exceptions)
		: exceptions(_exceptions) {

	}

	void dispatch_instruction(const halfword instr, const halfword second_instruction) {

		if(is_nop(instr)) dispatch(nop());
		//fprintf(stderr, "%s\n", instr.to_string().c_str());

		// Shift (immediate), add, subtract, move, and compare
		if(is_lsl_imm(instr)) {
			if(0 == instr.uint(6,5)) {
				// imm5 is zero, encoding is movs
				dispatch(movs(instr));
			} else {
				dispatch(lsl_imm(instr));
			}
		} else if(is_lsr_imm(instr)) {
			dispatch(lsr_imm(instr));
		} else if(is_asr_imm(instr)) {
			dispatch(asr_imm(instr));
		} else if(is_add_reg(instr)) {
			dispatch(add_reg(instr));
		} else if(is_subs_reg(instr)) {
			dispatch(subs_reg(instr));
		} else if(is_add_imm(instr)) {
			dispatch(add_imm(instr));
		} else if(is_subs_imm(instr)) {
			dispatch(subs_imm(instr));
		} else if(is_mov_imm(instr)) {
			dispatch(mov_imm(instr));
		} else if(is_cmp_imm(instr)) {
			dispatch(cmp_imm(instr));
		} else if(is_add_imm_t2(instr)) {
			dispatch(add_imm_t2(instr));
		} else if(is_subs_imm8(instr)) {
			dispatch(subs_imm8(instr));



			// Data processing
		} else if(is_and_reg(instr)) {
			dispatch(and_reg(instr));
		} else if(is_xor_reg(instr)) {
			dispatch(eor_reg(instr));
		} else if(is_lsl_reg(instr)) {
			dispatch(lsl_reg(instr));
		} else if(is_lsr_reg(instr)) {
			dispatch(lsr_reg(instr));
		} else if(is_asr_reg(instr)) {
			dispatch(asr_reg(instr));
		} else if(is_adc_reg(instr)) {
			dispatch(adc(instr));
		} else if(is_sub_c_reg(instr)) {
			dispatch(sub_c_reg(instr));
		} else if(is_ror_reg(instr)) {
			dispatch(ror_reg(instr));
		} else if(is_tst_reg(instr)) {
			dispatch(tst_reg(instr));
		} else if(is_rsb_imm(instr)) {
			dispatch(rsb_imm(instr));
		} else if(is_cmp_reg(instr)) {
			dispatch(cmp_reg(instr));
		} else if(is_cmn_reg(instr)) {
			dispatch(cmn_reg(instr));
		} else if(is_lor_reg(instr)) {
			dispatch(lor_reg(instr));
		} else if(is_mul_reg(instr)) {
			dispatch(mul_reg(instr));
		} else if(is_bitclear_reg(instr)) {
			dispatch(bic_reg(instr));
		} else if(is_not_reg(instr)) {
			dispatch(not_reg(instr));


			// Special data instructions and branch and exchange
		} else if(is_add_highreg(instr)) {
			dispatch(add_highreg(instr));
		} else if(is_sdibe_unpredictable_0(instr)) {
			// fault !
		} else if(is_cmp_highreg(instr)) {
			dispatch(cmp_highreg(instr));
		} else if(is_mov_highreg(instr)) {
			dispatch(mov_highreg(instr));
		} else if(is_bx(instr)) {
			dispatch(bx(instr));
		} else if(is_blx(instr)) {
			dispatch(blx(instr));
		} else if(is_ldr_literal(instr)) {
			dispatch(ldr_literal(instr));


			// load store
		} else if(is_store_reg_word_reg(instr)) {
			dispatch(store_reg_word_reg(instr));
		} else if(is_store_reg_halfword_reg(instr)) {
			dispatch(store_reg_halfword_reg(instr));
		} else if(is_store_reg_byte_reg(instr)) {
			dispatch(store_reg_byte_reg(instr));
		} else if(is_load_reg_sbyte_reg(instr)) {
			dispatch(load_reg_sbyte_reg(instr));
		} else if(is_ldr_reg(instr)) {
			dispatch(ldr_reg(instr));
		} else if(is_load_reg_halfword_reg(instr)) {
			dispatch(load_reg_halfword_reg(instr));
		} else if(is_load_reg_byte_reg(instr)) {
			dispatch(load_reg_byte_reg(instr));
		} else if(is_load_reg_shalfword_reg(instr)) {
			dispatch(load_reg_shalfword_reg(instr));
		} else if(is_store_word_imm(instr)) {
			dispatch(store_word_imm(instr));
		} else if(is_ldr_imm(instr)) {
			dispatch(ldr_imm(instr));
		} else if(is_store_byte_imm(instr)) {
			dispatch(store_byte_imm(instr));
		} else if(is_load_byte_imm(instr)) {
			dispatch(load_byte_imm(instr));
		} else if(is_store_halfword_imm(instr)) {
			dispatch(store_halfword_imm(instr));
		} else if(is_load_halfword_imm(instr)) {
			dispatch(load_halfword_imm(instr));
		} else if(is_store_word_sp_imm(instr)) {
			dispatch(store_word_sp_imm(instr));
		} else if(is_ldr_imm_sp(instr)) {
			dispatch(ldr_imm_sp(instr));


			// PC relative address
		} else if(is_adr(instr)) {
			dispatch(adr(instr));

			// Generate SP-relative address: ADD (SP plus immediate)
		} else if(is_add_sp_imm(instr)) {
			dispatch(add_sp_imm(instr));


			// Miscellaneous 16-bit instructions on page A5-8
		} else if(is_add_sp_imm_t2(instr)) {
			dispatch(add_sp_imm_t2(instr));
		} else if(is_sub_sp_imm(instr)) {
			dispatch(sub_sp_imm(instr));
		} else if(is_sxth(instr)) {
			dispatch(sxth(instr));
		} else if(is_sxtb(instr)) {
			dispatch(sxtb(instr));
		} else if(is_uxth(instr)) {
			dispatch(uxth(instr));
		} else if(is_uxtb(instr)) {
			dispatch(uxtb(instr));
		} else if(is_push(instr)) {
			dispatch(push(instr));
		} else if(is_cps(instr)) {
			//TODO: cps
			fprintf(stderr, "CPS is unimplemented\n");
		} else if(is_rev_word(instr)) {
			dispatch(rev_word(instr));
		} else if(is_rev_packed_halfword(instr)) {
			dispatch(rev_packed_halfword(instr));
		} else if(is_rev_signed_halfword(instr)) {
			dispatch(rev_packed_signed_halfword(instr));
		} else if(is_pop(instr)) {
			dispatch(pop(instr));
		} else if(is_breakpoint(instr)) {
			// TODO:
			fprintf(stderr, "BKP unimplemented\n");
		} else if(is_hints(instr)) {
			// TODO:
			fprintf(stderr, "HINTS unimplemented\n");


			// Store multiple registers, see STM, STMIA, STMEA on page A6-175
		} else if(is_stm(instr)) {
			dispatch(stm(instr));

			// Load multiple registers, see LDM, LDMIA, LDMFD on page A6-137
		} else if(is_ldm(instr)) {
			dispatch(ldm(instr));


		} else if(is_branch(instr)) {
			fprintf(stderr, "%s\n", branch(instr).to_string().c_str());
			dispatch(branch(instr));
		} else if(is_svc(instr)) {
			dispatch(svc(instr));
		} else if(is_unconditional_branch(instr)) {
			dispatch(unconditional_branch(instr));


		// 32bit encodings
		} else if(is_32bit_thumb_encoding(instr)) {

			if(is_undefined32(instr, second_instruction)) {
				exceptions[exception::HARDFAULT] = true;
			} else if(is_32bit_thumb_br_misc_ctl(instr, second_instruction)) {

				if(is_32bit_thumb_msr(instr, second_instruction)) {
					fprintf(stderr, "uMSR unimplemented\n");
				} else if(is_32bit_thumb_misc_ctl(instr, second_instruction)) {
					fprintf(stderr, "unimplemented 32 bit misc ctl intructions\n");
				} else if(is_32bit_thumb_mrs(instr, second_instruction)) {
					fprintf(stderr, "unimplemented mrs\n");
				} else if(is_32bit_thumb_bl(instr, second_instruction)) {
					dispatch(bl_imm(instr, second_instruction));
				} else {
					fprintf(stderr, "unimplemented 32bit misc br and ctrl instruction\n");
				}
			} else {
				fprintf(stderr, "undefined 32bit instruction\n");
				exceptions[exception::HARDFAULT] = true;
			}

		} else if(is_undefined(instr)) {
			exceptions[exception::HARDFAULT] = true;
		} else {
			fprintf(stderr, "unhandled instruction %04X\n", (uint32_t)instr);
			fprintf(stderr, "unimplemented\n");
		}
	}

private:

	virtual void dispatch(const nop& instruction) = 0;
	virtual void dispatch(const lsl_imm& instruction) = 0;
	virtual void dispatch(const lsr_imm& instruction) = 0;
	virtual void dispatch(const asr_imm& instruction) = 0;
	virtual void dispatch(const add_reg& instruction) = 0;
	virtual void dispatch(const subs_reg& instruction) = 0;
	virtual void dispatch(const add_imm& instruction) = 0;
	virtual void dispatch(const subs_imm& instruction) = 0;
	virtual void dispatch(const mov_imm& instruction) = 0;
	virtual void dispatch(const movs& instruction) = 0;
	virtual void dispatch(const cmp_imm& instruction) = 0;
	virtual void dispatch(const add_imm_t2& instruction) = 0;
	virtual void dispatch(const subs_imm8& instruction) = 0;
	virtual void dispatch(const and_reg& instruction) = 0;
	virtual void dispatch(const eor_reg& instruction) = 0;
	virtual void dispatch(const lsl_reg& instruction) = 0;
	virtual void dispatch(const lsr_reg& instruction) = 0;
	virtual void dispatch(const asr_reg& instruction) = 0;
	virtual void dispatch(const adc& instruction) = 0;
	virtual void dispatch(const sub_c_reg& instruction) = 0;
	virtual void dispatch(const ror_reg& instruction) = 0;
	virtual void dispatch(const tst_reg& instruction) = 0;
	virtual void dispatch(const rsb_imm& instruction) = 0;
	virtual void dispatch(const cmp_reg& instruction) = 0;
	virtual void dispatch(const cmn_reg& instruction) = 0;
	virtual void dispatch(const lor_reg& instruction) = 0;
	virtual void dispatch(const mul_reg& instruction) = 0;
	virtual void dispatch(const bic_reg& instruction) = 0;
	virtual void dispatch(const not_reg& instruction) = 0;
	virtual void dispatch(const add_highreg& instruction) = 0;
	virtual void dispatch(const cmp_highreg& instruction) = 0;
	virtual void dispatch(const mov_highreg& instruction) = 0;
	virtual void dispatch(const bx& instruction) = 0;
	virtual void dispatch(const blx& instruction) = 0;
	virtual void dispatch(const ldr_literal& instruction) = 0;
	virtual void dispatch(const store_reg_word_reg& instruction) = 0;
	virtual void dispatch(const store_reg_halfword_reg& instruction) = 0;
	virtual void dispatch(const store_reg_byte_reg& instruction) = 0;
	virtual void dispatch(const load_reg_sbyte_reg& instruction) = 0;
	virtual void dispatch(const ldr_reg& instruction) = 0;
	virtual void dispatch(const load_reg_halfword_reg& instruction) = 0;
	virtual void dispatch(const load_reg_byte_reg& instruction) = 0;
	virtual void dispatch(const load_reg_shalfword_reg& instruction) = 0;
	virtual void dispatch(const store_word_imm& instruction) = 0;
	virtual void dispatch(const ldr_imm& instruction) = 0;
	virtual void dispatch(const store_byte_imm& instruction) = 0;
	virtual void dispatch(const load_byte_imm& instruction) = 0;
	virtual void dispatch(const store_halfword_imm& instruction) = 0;
	virtual void dispatch(const load_halfword_imm& instruction) = 0;
	virtual void dispatch(const store_word_sp_imm& instruction) = 0;
	virtual void dispatch(const ldr_imm_sp& instruction) = 0;
	virtual void dispatch(const adr& instruction) = 0;
	virtual void dispatch(const add_sp_imm& instruction) = 0;
	virtual void dispatch(const add_sp_imm_t2& instruction) = 0;
	virtual void dispatch(const sub_sp_imm& instruction) = 0;
	virtual void dispatch(const sxth& instruction) = 0;
	virtual void dispatch(const sxtb& instruction) = 0;
	virtual void dispatch(const uxth& instruction) = 0;
	virtual void dispatch(const uxtb& instruction) = 0;
	virtual void dispatch(const push& instruction) = 0;
	virtual void dispatch(const pop& instruction) = 0;
	virtual void dispatch(const rev_word& instruction) = 0;
	virtual void dispatch(const rev_packed_halfword& instruction) = 0;
	virtual void dispatch(const rev_packed_signed_halfword& instruction) = 0;
	virtual void dispatch(const branch& instruction) = 0;
	virtual void dispatch(const unconditional_branch& instruction) = 0;
	virtual void dispatch(const stm& instruction) = 0;
	virtual void dispatch(const ldm& instruction) = 0;
	virtual void dispatch(const bl_imm& instruction) = 0;
	virtual void dispatch(const svc& instruction) = 0;
};


#endif //MICROMACHINE_DISPATCH_HPP
