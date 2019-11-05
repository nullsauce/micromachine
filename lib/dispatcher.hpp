#ifndef MICROMACHINE_DISPATCH_HPP
#define MICROMACHINE_DISPATCH_HPP

#include "instruction_pair.hpp"
#include "instructions.hpp"
#include "exception_vector.hpp"

namespace {

	static bool is_siadsumoco(const uint16_t instruction) {
		return 0 == bits<14,2>::of(instruction);
	}

	static bool is_data_processing(const uint16_t instruction) {
		return 0b010000 == bits<10,6>::of(instruction);
	}

	static bool is_sdibe(const uint16_t instruction) {
		return 0b010001 == bits<10,6>::of(instruction);
	}

	static bool is_lsl_imm(const uint16_t instruction) {
		return is_siadsumoco(instruction) &&
			   0b000 == bits<11,3>::of(instruction);

	}

	static bool is_lsr_imm(const uint16_t instruction) {
		return is_siadsumoco(instruction) &&
			   0b001 == bits<11,3>::of(instruction);
	}

	static bool is_asr_imm(const uint16_t instruction) {
		return is_siadsumoco(instruction) &&
			   0b010 == bits<11,3>::of(instruction);
	}

	static bool is_add_reg(const uint16_t instruction) {
		return is_siadsumoco(instruction) &&
			   0b01100 == bits<9,5>::of(instruction);
	}

	static bool is_subs_reg(const uint16_t instruction) {
		return is_siadsumoco(instruction) &&
			   0b01101 == bits<9,5>::of(instruction);
	}

	static bool is_add_imm(const uint16_t instruction) {
		return is_siadsumoco(instruction) &&
			   0b01110 == bits<9,5>::of(instruction);
	}

	static bool is_subs_imm(const uint16_t instruction) {
		return is_siadsumoco(instruction) &&
			   0b01111 == bits<9,5>::of(instruction);
	}

	static bool is_mov_imm(const uint16_t instruction) {
		return is_siadsumoco(instruction) &&
			   0b100 == bits<11,3>::of(instruction);
	}

	static bool is_cmp_imm(const uint16_t instruction) {
		return is_siadsumoco(instruction) &&
			   0b101 == bits<11,3>::of(instruction);
	}

	static bool is_add_imm_t2(const uint16_t instruction) {
		return is_siadsumoco(instruction) &&
			   0b110 == bits<11,3>::of(instruction);
	}

	static bool is_subs_imm8(const uint16_t instruction) {
		return is_siadsumoco(instruction) &&
			   0b111 == bits<11,3>::of(instruction);
	}


	static bool is_and_reg(const uint16_t instruction) {
		return is_data_processing(instruction) &&
			   0b0000 == bits<6,4>::of(instruction);
	}

	static bool is_xor_reg(const uint16_t instruction) {
		return is_data_processing(instruction) &&
			   0b0001 == bits<6,4>::of(instruction);
	}

	static bool is_lsl_reg(const uint16_t instruction) {
		return is_data_processing(instruction) &&
			   0b0010 == bits<6,4>::of(instruction);
	}

	static bool is_lsr_reg(const uint16_t instruction) {
		return is_data_processing(instruction) &&
			   0b0011 == bits<6,4>::of(instruction);
	}

	static bool is_asr_reg(const uint16_t instruction) {
		return is_data_processing(instruction) &&
			   0b0100 == bits<6,4>::of(instruction);
	}

	static bool is_adc_reg(const uint16_t instruction) {
		return is_data_processing(instruction) &&
			   0b0101 == bits<6,4>::of(instruction);
	}

	static bool is_sbc(const uint16_t instruction) {
		return is_data_processing(instruction) &&
			   0b0110 == bits<6,4>::of(instruction);
	}

	static bool is_ror_reg(const uint16_t instruction) {
		return is_data_processing(instruction) &&
			   0b0111 == bits<6,4>::of(instruction);
	}

	static bool is_tst_reg(const uint16_t instruction) {
		return is_data_processing(instruction) &&
			   0b1000 == bits<6,4>::of(instruction);
	}

	static bool is_rsb_imm(const uint16_t instruction) {
		return is_data_processing(instruction) &&
			   0b1001 == bits<6,4>::of(instruction);
	}

	static bool is_cmn_reg(const uint16_t instruction) {
		return is_data_processing(instruction) &&
			   0b1011 == bits<6,4>::of(instruction);
	}

	static bool is_cmp_reg(const uint16_t instruction) {
		return is_data_processing(instruction) &&
			   0b1010 == bits<6,4>::of(instruction);
	}

	static bool is_orr_reg(const uint16_t instruction) {
		return is_data_processing(instruction) &&
			   0b1100 == bits<6,4>::of(instruction);
	}

	static bool is_mul_reg(const uint16_t instruction) {
		return is_data_processing(instruction) &&
			   0b1101 == bits<6,4>::of(instruction);
	}

	static bool is_bitclear_reg(const uint16_t instruction) {
		return is_data_processing(instruction) &&
			   0b1110 == bits<6,4>::of(instruction);
	}

	static bool is_not_reg(const uint16_t instruction) {
		return is_data_processing(instruction) &&
			   0b1111 == bits<6,4>::of(instruction);
	}


	static bool is_add_highreg(const uint16_t instruction) {
		return is_sdibe(instruction) &&
			   0b00 == bits<8,2>::of(instruction);
	}

	static bool is_sdibe_unpredictable_0(const uint16_t instruction) {
		return is_sdibe(instruction) &&
			   0b0100 == bits<6,4>::of(instruction);
	}

	static bool is_cmp_highreg(const uint16_t instruction) {
		return is_sdibe(instruction) && (
			(0b0101 == bits<6,4>::of(instruction)) ||
			(0b011 == bits<7,3>::of(instruction))
		);
	}

	static bool is_mov_highreg(const uint16_t instruction) {
		return is_sdibe(instruction) &&
			   0b10 == bits<8,2>::of(instruction);
	}

	static bool is_bx(const uint16_t instruction) {
		return is_sdibe(instruction) &&
			   0b110 == bits<7,3>::of(instruction);
	}

	static bool is_blx(const uint16_t instruction) {
		return is_sdibe(instruction) &&
			   0b111 == bits<7,3>::of(instruction);
	}

	static bool is_ldr_literal(const uint16_t instruction) {
		return 0b01001 == bits<11,5>::of(instruction);
	}

	static bool is_str_reg(const uint16_t instruction) {
		return 0b0101000 == bits<9,7>::of(instruction);
	}

	static bool is_strh_reg(const uint16_t instruction) {
		return 0b0101001 == bits<9,7>::of(instruction);
	}

	static bool is_strb_reg(const uint16_t instruction) {
		return 0b0101010 == bits<9,7>::of(instruction);
	}

	static bool is_ldrsb_reg(const uint16_t instruction) {
		return 0b0101011 == bits<9,7>::of(instruction);
	}

	static bool is_ldr_reg(const uint16_t instruction) {
		return 0b0101100 == bits<9,7>::of(instruction);
	}

	static bool is_ldrh_reg(const uint16_t instruction) {
		return 0b0101101 == bits<9,7>::of(instruction);
	}

	static bool is_ldrb_reg(const uint16_t instruction) {
		return 0b0101110 == bits<9,7>::of(instruction);
	}

	static bool is_ldrsh_reg(const uint16_t instruction) {
		return 0b0101111 == bits<9,7>::of(instruction);
	}

	static bool is_str_imm(const uint16_t instruction) {
		return 0b01100 == bits<11,5>::of(instruction);
	}

	static bool is_ldr_imm(const uint16_t instruction) {
		return 0b01101 == bits<11,5>::of(instruction);
	}

	static bool is_strb_imm(const uint16_t instruction) {
		return 0b01110 == bits<11,5>::of(instruction);
	}

	static bool is_ldrb_imm(const uint16_t instruction) {
		return 0b01111 == bits<11,5>::of(instruction);
	}

	static bool is_strh_imm(const uint16_t instruction) {
		return 0b10000 == bits<11,5>::of(instruction);
	}

	static bool is_ldrh_imm(const uint16_t instruction) {
		return 0b10001 == bits<11,5>::of(instruction);
	}

	static bool is_str_sp_imm(const uint16_t instruction) {
		return 0b10010 == bits<11,5>::of(instruction);
	}

	static bool is_ldr_sp_imm(const uint16_t instruction) {
		return 0b10011 == bits<11,5>::of(instruction);
	}

	static bool is_adr(const uint16_t instruction) {
		return 0b10100 == bits<11,5>::of(instruction);
	}

	static bool is_add_sp_imm(const uint16_t instruction) {
		return 0b10101 == bits<11,5>::of(instruction);
	}

	static bool is_add_sp_imm_t2(const uint16_t instruction) {
		return 0b101100000 == bits<7,9>::of(instruction);
	}

	static bool is_sub_sp_imm(const uint16_t instruction) {
		return 0b101100001 == bits<7,9>::of(instruction);
	}

	static bool is_sxth(const uint16_t instruction) {
		return 0b1011001000 == bits<6,10>::of(instruction);
	}

	static bool is_sxtb(const uint16_t instruction) {
		return 0b1011001001 == bits<6,10>::of(instruction);
	}

	static bool is_uxth(const uint16_t instruction) {
		return 0b1011001010 == bits<6,10>::of(instruction);
	}

	static bool is_uxtb(const uint16_t instruction) {
		return 0b1011001011 == bits<6,10>::of(instruction);
	}

	static bool is_push(const uint16_t instruction) {
		return 0b1011010 == bits<9,7>::of(instruction);
	}

	static bool is_cps(const uint16_t instruction) {
		return 0b10110110011 == bits<5,11>::of(instruction);
	}

	static bool is_rev_word(const uint16_t instruction) {
		return 0b1011101000 == bits<6,10>::of(instruction);
	}

	static bool is_rev16(const uint16_t instruction) {
		return 0b1011101001 == bits<6,10>::of(instruction);
	}

	static bool is_revsh(const uint16_t instruction) {
		return 0b1011101011 == bits<6,10>::of(instruction);
	}

	static bool is_pop(const uint16_t instruction) {
		return 0b1011110 == bits<9,7>::of(instruction);
	}

	static bool is_breakpoint(const uint16_t instruction) {
		return 0b10111110 == bits<8,8>::of(instruction);
	}

	static bool is_hints(const uint16_t instruction) {
		return 0b10111111 == bits<8,8>::of(instruction);
	}

	static bool is_nop(const uint16_t instruction) {
		return 0b1011111100000000 == instruction;
	}

	static bool is_stm(const uint16_t instruction) {
		return 0b11000 == bits<11,5>::of(instruction);
	}

	static bool is_ldm(const uint16_t instruction) {
		return 0b11001 == bits<11,5>::of(instruction);
	}

	static bool is_branch(const uint16_t instruction) {
		return 0b1101 == bits<12,4>::of(instruction) &&
			   (bits<9,3>::of(instruction) != 0b111);
	}

	static bool is_svc(const uint16_t instruction) {
		return 0b1101 == bits<12,4>::of(instruction) &&
			   0b1111 == bits<8,4>::of(instruction);
	}


	static bool is_unconditional_branch(const uint16_t instruction) {
		return 0b11100 == bits<11,5>::of(instruction);
	}

	static bool is_32bit_thumb_encoding(const uint16_t instruction) {
		return 0b111 == bits<13,3>::of(instruction) &&
			   0b00 !=  bits<11,2>::of(instruction);
	}


	static bool is_32bit_thumb_br_misc_ctl(const instruction_pair instr) {
		return is_32bit_thumb_encoding(instr.first()) &&
			   0b10 == bits<11,2>::of(instr.first()) &&
			   0b1  == bits<15,1>::of(instr.second());
	}

	static bool is_32bit_thumb_msr(const instruction_pair instr) {
		return is_32bit_thumb_br_misc_ctl(instr) &&
			   0b011100 == bits<5,6>::of(instr.first()) &&
			   0b000 	== (bits<12,3>::of(instr.second()) & 0b101);
	}

	static bool is_32bit_thumb_misc_ctl(const instruction_pair instr) {
		return is_32bit_thumb_br_misc_ctl(instr) &&
			   0b0111011 == bits<4,7>::of(instr.first()) &&
			   0b000 	== (bits<12,3>::of(instr.second()) & 0b101);
	}

	static bool is_32bit_thumb_mrs(const instruction_pair instr) {
		return is_32bit_thumb_br_misc_ctl(instr) &&
			   0b011111 == bits<5,6>::of(instr.first()) &&
			   0b000 	== (bits<12,3>::of(instr.second()) & 0b101);
	}

	static bool is_32bit_thumb_bl(const instruction_pair instr) {
		auto op2 = bits<12,3>::of(instr.second()) & 0b101;
		return is_32bit_thumb_br_misc_ctl(instr) &&
			 0b101 	== op2 ;
	}

	static bool is_undefined(const uint16_t instr) {
		return 0b11011110 == bits<8,8>::of(instr);
	}

	static bool is_undefined32(const instruction_pair instr) {
		return is_32bit_thumb_encoding(instr.first()) &&
				0b111101111111 == bits<4,12>::of(instr.first()) &&
				0b1010 == bits<12,4>::of(instr.second());
	}
}

class dispatcher {


public:

	void dispatch_instruction(const instruction_pair instruction_pair) {

		const uint16_t instr = instruction_pair.first();
		if(is_nop(instr)) dispatch(nop());
		//fprintf(stderr, "%s\n", instr.to_string().c_str());

		// Shift (immediate), add, subtract, move, and compare
		if(is_lsl_imm(instr)) {
			if(0 == bits<6,5>::of(instr)) {
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
		} else if(is_sbc(instr)) {
			dispatch(sbc(instr));
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
		} else if(is_orr_reg(instr)) {
			dispatch(orr_reg(instr));
		} else if(is_mul_reg(instr)) {
			dispatch(mul_reg(instr));
		} else if(is_bitclear_reg(instr)) {
			dispatch(bic_reg(instr));
		} else if(is_not_reg(instr)) {
			dispatch(mvn(instr));


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
		} else if(is_str_reg(instr)) {
			dispatch(str_reg(instr));
		} else if(is_strh_reg(instr)) {
			dispatch(strh_reg(instr));
		} else if(is_strb_reg(instr)) {
			dispatch(strb_reg(instr));
		} else if(is_ldrsb_reg(instr)) {
			dispatch(ldrsb_reg(instr));
		} else if(is_ldr_reg(instr)) {
			dispatch(ldr_reg(instr));
		} else if(is_ldrh_reg(instr)) {
			dispatch(ldrh_reg(instr));
		} else if(is_ldrb_reg(instr)) {
			dispatch(ldrb_reg(instr));
		} else if(is_ldrsh_reg(instr)) {
			dispatch(ldrsh_reg(instr));
		} else if(is_str_imm(instr)) {
			dispatch(str_imm(instr));
		} else if(is_ldr_imm(instr)) {
			dispatch(ldr_imm(instr));
		} else if(is_strb_imm(instr)) {
			dispatch(strb_imm(instr));
		} else if(is_ldrb_imm(instr)) {
			dispatch(ldrb_imm(instr));
		} else if(is_strh_imm(instr)) {
			dispatch(strh_imm(instr));
		} else if(is_ldrh_imm(instr)) {
			dispatch(ldrh_imm(instr));
		} else if(is_str_sp_imm(instr)) {
			dispatch(str_sp_imm(instr));
		} else if(is_ldr_sp_imm(instr)) {
			dispatch(ldr_sp_imm(instr));


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
			dispatch(cps(instr));
		} else if(is_rev_word(instr)) {
			dispatch(rev_word(instr));
		} else if(is_rev16(instr)) {
			dispatch(rev16(instr));
		} else if(is_revsh(instr)) {
			dispatch(revsh(instr));
		} else if(is_pop(instr)) {
			dispatch(pop(instr));
		} else if(is_breakpoint(instr)) {
			dispatch(bkpt(instr));
		} else if(is_hints(instr)) {
			hint instruction(instr);
			if(0b0000 != instruction.opb()) {
				// undefined
			} else {
				switch((uint8_t)instruction.opa()) {
					case 0b0000: {
						dispatch(nop());
						break;
					}
					case 0b0001: {
						dispatch(yield());
						break;
					}
					case 0b0010: {
						dispatch(wfe());
						break;
					}
					case 0b0011: {
						dispatch(wfi());
						break;
					}
					case 0b0100: {
						dispatch(sev());
						break;
					}
					default: {
						// undefined
					}
				}
			}

			// Store multiple registers, see STM, STMIA, STMEA on page A6-175
		} else if(is_stm(instr)) {
			dispatch(stm(instr));

			// Load multiple registers, see LDM, LDMIA, LDMFD on page A6-137
		} else if(is_ldm(instr)) {
			dispatch(ldm(instr));


		} else if(is_branch(instr)) {
			//fprintf(stderr, "%s\n", branch(instr).to_string().c_str());
			dispatch(branch(instr));
		} else if(is_svc(instr)) {
			dispatch(svc(instr));
		} else if(is_unconditional_branch(instr)) {
			dispatch(unconditional_branch(instr));


		// 32bit encodings
		} else if(is_32bit_thumb_encoding(instr)) {

			if(is_undefined32(instruction_pair)) {
				dispatch(udfw(instruction_pair));
			} else if(is_32bit_thumb_br_misc_ctl(instruction_pair)) {

				if(is_32bit_thumb_msr(instruction_pair)) {
					dispatch(msr(instruction_pair));
				} else if(is_32bit_thumb_misc_ctl(instruction_pair)) {
					//fprintf(stderr, "unimplemented 32 bit misc ctl intructions\n");
				} else if(is_32bit_thumb_mrs(instruction_pair)) {
					dispatch(mrs(instruction_pair));
				} else if(is_32bit_thumb_bl(instruction_pair)) {
					dispatch(bl_imm(instruction_pair));
				} else {
					//fprintf(stderr, "unimplemented 32bit misc br and ctrl instruction\n");
					invalid_instruction(instruction_pair);
				}
			} else {
				/*
				fprintf(stderr, "undefined 32bit instruction %04x %04x\n",
					(uint16_t)instruction_pair.first(),
					(uint16_t)instruction_pair.second()
				);*/
				invalid_instruction(instruction_pair);
			}

		} else if(is_undefined(instr)) {
			dispatch(udf(instr));
		} else {
			//fprintf(stderr, "unhandled instruction %04X\n", (uint32_t)instr);
			//fprintf(stderr, "unimplemented\n");
			invalid_instruction(instr);
		}
	}

private:
	virtual void invalid_instruction(const uint16_t instr) = 0;
	virtual void invalid_instruction(const instruction_pair instr) = 0;
	virtual void dispatch(const nop instruction) = 0;
	virtual void dispatch(const yield instruction) = 0;
	virtual void dispatch(const wfe instruction) = 0;
	virtual void dispatch(const wfi instruction) = 0;
	virtual void dispatch(const sev instruction) = 0;
	virtual void dispatch(const lsl_imm instruction) = 0;
	virtual void dispatch(const lsr_imm instruction) = 0;
	virtual void dispatch(const asr_imm instruction) = 0;
	virtual void dispatch(const add_reg instruction) = 0;
	virtual void dispatch(const subs_reg instruction) = 0;
	virtual void dispatch(const add_imm instruction) = 0;
	virtual void dispatch(const subs_imm instruction) = 0;
	virtual void dispatch(const mov_imm instruction) = 0;
	virtual void dispatch(const movs instruction) = 0;
	virtual void dispatch(const cmp_imm instruction) = 0;
	virtual void dispatch(const add_imm_t2 instruction) = 0;
	virtual void dispatch(const subs_imm8 instruction) = 0;
	virtual void dispatch(const and_reg instruction) = 0;
	virtual void dispatch(const eor_reg instruction) = 0;
	virtual void dispatch(const lsl_reg instruction) = 0;
	virtual void dispatch(const lsr_reg instruction) = 0;
	virtual void dispatch(const asr_reg instruction) = 0;
	virtual void dispatch(const adc instruction) = 0;
	virtual void dispatch(const sbc instruction) = 0;
	virtual void dispatch(const ror_reg instruction) = 0;
	virtual void dispatch(const tst_reg instruction) = 0;
	virtual void dispatch(const rsb_imm instruction) = 0;
	virtual void dispatch(const cmp_reg instruction) = 0;
	virtual void dispatch(const cmn_reg instruction) = 0;
	virtual void dispatch(const orr_reg instruction) = 0;
	virtual void dispatch(const mul_reg instruction) = 0;
	virtual void dispatch(const bic_reg instruction) = 0;
	virtual void dispatch(const mvn instruction) = 0;
	virtual void dispatch(const add_highreg instruction) = 0;
	virtual void dispatch(const cmp_highreg instruction) = 0;
	virtual void dispatch(const mov_highreg instruction) = 0;
	virtual void dispatch(const bx instruction) = 0;
	virtual void dispatch(const blx instruction) = 0;
	virtual void dispatch(const ldr_literal instruction) = 0;
	virtual void dispatch(const str_reg instruction) = 0;
	virtual void dispatch(const strh_reg instruction) = 0;
	virtual void dispatch(const strb_reg instruction) = 0;
	virtual void dispatch(const ldrsb_reg instruction) = 0;
	virtual void dispatch(const ldr_reg instruction) = 0;
	virtual void dispatch(const ldrh_reg instruction) = 0;
	virtual void dispatch(const ldrb_reg instruction) = 0;
	virtual void dispatch(const ldrsh_reg instruction) = 0;
	virtual void dispatch(const str_imm instruction) = 0;
	virtual void dispatch(const ldr_imm instruction) = 0;
	virtual void dispatch(const strb_imm instruction) = 0;
	virtual void dispatch(const ldrb_imm instruction) = 0;
	virtual void dispatch(const strh_imm instruction) = 0;
	virtual void dispatch(const ldrh_imm instruction) = 0;
	virtual void dispatch(const str_sp_imm instruction) = 0;
	virtual void dispatch(const ldr_sp_imm instruction) = 0;
	virtual void dispatch(const adr instruction) = 0;
	virtual void dispatch(const add_sp_imm instruction) = 0;
	virtual void dispatch(const add_sp_imm_t2 instruction) = 0;
	virtual void dispatch(const sub_sp_imm instruction) = 0;
	virtual void dispatch(const sxth instruction) = 0;
	virtual void dispatch(const sxtb instruction) = 0;
	virtual void dispatch(const uxth instruction) = 0;
	virtual void dispatch(const uxtb instruction) = 0;
	virtual void dispatch(const push instruction) = 0;
	virtual void dispatch(const cps instruction) = 0;
	virtual void dispatch(const pop instruction) = 0;
	virtual void dispatch(const bkpt instr) = 0;
	virtual void dispatch(const rev_word instruction) = 0;
	virtual void dispatch(const rev16 instruction) = 0;
	virtual void dispatch(const revsh instruction) = 0;
	virtual void dispatch(const branch instruction) = 0;
	virtual void dispatch(const unconditional_branch instruction) = 0;
	virtual void dispatch(const stm instruction) = 0;
	virtual void dispatch(const ldm instruction) = 0;
	virtual void dispatch(const mrs instruction) = 0;
	virtual void dispatch(const msr instruction) = 0;
	virtual void dispatch(const bl_imm instruction) = 0;
	virtual void dispatch(const svc instruction) = 0;
	virtual void dispatch(const udf instr) = 0;
	virtual void dispatch(const udfw instr) = 0;
};


#endif //MICROMACHINE_DISPATCH_HPP
