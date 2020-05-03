#pragma once

#include "instruction_pair.hpp"
#include "instructions.hpp"
#include "types/bits.hpp"

namespace {

using namespace micromachine::system;

static bool is_siadsumoco(const uint16_t instruction) {
	return 0 == bits<14, 2>::of(instruction);
}

static bool is_data_processing(const uint16_t instruction) {
	return 0b010000 == bits<10, 6>::of(instruction);
}

static bool is_sdibe(const uint16_t instruction) {
	return 0b010001 == bits<10, 6>::of(instruction);
}

bool is_lsl_imm(const uint16_t instruction) {
	return is_siadsumoco(instruction) && 0b000 == bits<11, 3>::of(instruction);
}

bool is_lsr_imm(const uint16_t instruction) {
	return is_siadsumoco(instruction) && 0b001 == bits<11, 3>::of(instruction);
}

bool is_asr_imm(const uint16_t instruction) {
	return is_siadsumoco(instruction) && 0b010 == bits<11, 3>::of(instruction);
}

bool is_add_reg(const uint16_t instruction) {
	return is_siadsumoco(instruction) && 0b01100 == bits<9, 5>::of(instruction);
}

bool is_subs_reg(const uint16_t instruction) {
	return is_siadsumoco(instruction) && 0b01101 == bits<9, 5>::of(instruction);
}

bool is_add_imm(const uint16_t instruction) {
	return is_siadsumoco(instruction) && 0b01110 == bits<9, 5>::of(instruction);
}

bool is_subs_imm(const uint16_t instruction) {
	return is_siadsumoco(instruction) && 0b01111 == bits<9, 5>::of(instruction);
}

bool is_mov_imm(const uint16_t instruction) {
	return is_siadsumoco(instruction) && 0b100 == bits<11, 3>::of(instruction);
}

bool is_cmp_imm(const uint16_t instruction) {
	return is_siadsumoco(instruction) && 0b101 == bits<11, 3>::of(instruction);
}

bool is_add_imm_t2(const uint16_t instruction) {
	return is_siadsumoco(instruction) && 0b110 == bits<11, 3>::of(instruction);
}

bool is_subs_imm8(const uint16_t instruction) {
	return is_siadsumoco(instruction) && 0b111 == bits<11, 3>::of(instruction);
}

bool is_and_reg(const uint16_t instruction) {
	return is_data_processing(instruction) && 0b0000 == bits<6, 4>::of(instruction);
}

bool is_xor_reg(const uint16_t instruction) {
	return is_data_processing(instruction) && 0b0001 == bits<6, 4>::of(instruction);
}

bool is_lsl_reg(const uint16_t instruction) {
	return is_data_processing(instruction) && 0b0010 == bits<6, 4>::of(instruction);
}

bool is_lsr_reg(const uint16_t instruction) {
	return is_data_processing(instruction) && 0b0011 == bits<6, 4>::of(instruction);
}

bool is_asr_reg(const uint16_t instruction) {
	return is_data_processing(instruction) && 0b0100 == bits<6, 4>::of(instruction);
}

bool is_adc_reg(const uint16_t instruction) {
	return is_data_processing(instruction) && 0b0101 == bits<6, 4>::of(instruction);
}

bool is_sbc(const uint16_t instruction) {
	return is_data_processing(instruction) && 0b0110 == bits<6, 4>::of(instruction);
}

bool is_ror_reg(const uint16_t instruction) {
	return is_data_processing(instruction) && 0b0111 == bits<6, 4>::of(instruction);
}

bool is_tst_reg(const uint16_t instruction) {
	return is_data_processing(instruction) && 0b1000 == bits<6, 4>::of(instruction);
}

bool is_rsb_imm(const uint16_t instruction) {
	return is_data_processing(instruction) && 0b1001 == bits<6, 4>::of(instruction);
}

bool is_cmn_reg(const uint16_t instruction) {
	return is_data_processing(instruction) && 0b1011 == bits<6, 4>::of(instruction);
}

bool is_cmp_reg(const uint16_t instruction) {
	return is_data_processing(instruction) && 0b1010 == bits<6, 4>::of(instruction);
}

bool is_orr_reg(const uint16_t instruction) {
	return is_data_processing(instruction) && 0b1100 == bits<6, 4>::of(instruction);
}

bool is_mul_reg(const uint16_t instruction) {
	return is_data_processing(instruction) && 0b1101 == bits<6, 4>::of(instruction);
}

bool is_bitclear_reg(const uint16_t instruction) {
	return is_data_processing(instruction) && 0b1110 == bits<6, 4>::of(instruction);
}

bool is_not_reg(const uint16_t instruction) {
	return is_data_processing(instruction) && 0b1111 == bits<6, 4>::of(instruction);
}

bool is_add_highreg(const uint16_t instruction) {
	return is_sdibe(instruction) && 0b00 == bits<8, 2>::of(instruction);
}

bool is_sdibe_unpredictable_0(const uint16_t instruction) {
	return is_sdibe(instruction) && 0b0100 == bits<6, 4>::of(instruction);
}

bool is_cmp_highreg(const uint16_t instruction) {
	return is_sdibe(instruction) &&
		   ((0b0101 == bits<6, 4>::of(instruction)) || (0b011 == bits<7, 3>::of(instruction)));
}

bool is_mov_highreg(const uint16_t instruction) {
	return is_sdibe(instruction) && 0b10 == bits<8, 2>::of(instruction);
}

bool is_bx(const uint16_t instruction) {
	return is_sdibe(instruction) && 0b110 == bits<7, 3>::of(instruction);
}

bool is_blx(const uint16_t instruction) {
	return is_sdibe(instruction) && 0b111 == bits<7, 3>::of(instruction);
}

static bool is_ldr_literal(const uint16_t instruction) {
	return 0b01001 == bits<11, 5>::of(instruction);
}

static bool is_str_reg(const uint16_t instruction) {
	return 0b0101000 == bits<9, 7>::of(instruction);
}

static bool is_strh_reg(const uint16_t instruction) {
	return 0b0101001 == bits<9, 7>::of(instruction);
}

static bool is_strb_reg(const uint16_t instruction) {
	return 0b0101010 == bits<9, 7>::of(instruction);
}

static bool is_ldrsb_reg(const uint16_t instruction) {
	return 0b0101011 == bits<9, 7>::of(instruction);
}

static bool is_ldr_reg(const uint16_t instruction) {
	return 0b0101100 == bits<9, 7>::of(instruction);
}

static bool is_ldrh_reg(const uint16_t instruction) {
	return 0b0101101 == bits<9, 7>::of(instruction);
}

static bool is_ldrb_reg(const uint16_t instruction) {
	return 0b0101110 == bits<9, 7>::of(instruction);
}

static bool is_ldrsh_reg(const uint16_t instruction) {
	return 0b0101111 == bits<9, 7>::of(instruction);
}

static bool is_str_imm(const uint16_t instruction) {
	return 0b01100 == bits<11, 5>::of(instruction);
}

static bool is_ldr_imm(const uint16_t instruction) {
	return 0b01101 == bits<11, 5>::of(instruction);
}

static bool is_strb_imm(const uint16_t instruction) {
	return 0b01110 == bits<11, 5>::of(instruction);
}

static bool is_ldrb_imm(const uint16_t instruction) {
	return 0b01111 == bits<11, 5>::of(instruction);
}

static bool is_strh_imm(const uint16_t instruction) {
	return 0b10000 == bits<11, 5>::of(instruction);
}

static bool is_ldrh_imm(const uint16_t instruction) {
	return 0b10001 == bits<11, 5>::of(instruction);
}

static bool is_str_sp_imm(const uint16_t instruction) {
	return 0b10010 == bits<11, 5>::of(instruction);
}

static bool is_ldr_sp_imm(const uint16_t instruction) {
	return 0b10011 == bits<11, 5>::of(instruction);
}

static bool is_adr(const uint16_t instruction) {
	return 0b10100 == bits<11, 5>::of(instruction);
}

static bool is_add_sp_imm(const uint16_t instruction) {
	return 0b10101 == bits<11, 5>::of(instruction);
}

static bool is_add_sp_imm_t2(const uint16_t instruction) {
	return 0b101100000 == bits<7, 9>::of(instruction);
}

static bool is_sub_sp_imm(const uint16_t instruction) {
	return 0b101100001 == bits<7, 9>::of(instruction);
}

static bool is_sxth(const uint16_t instruction) {
	return 0b1011001000 == bits<6, 10>::of(instruction);
}

static bool is_sxtb(const uint16_t instruction) {
	return 0b1011001001 == bits<6, 10>::of(instruction);
}

static bool is_uxth(const uint16_t instruction) {
	return 0b1011001010 == bits<6, 10>::of(instruction);
}

static bool is_uxtb(const uint16_t instruction) {
	return 0b1011001011 == bits<6, 10>::of(instruction);
}

static bool is_push(const uint16_t instruction) {
	return 0b1011010 == bits<9, 7>::of(instruction);
}

static bool is_cps(const uint16_t instruction) {
	return 0b10110110011 == bits<5, 11>::of(instruction);
}

static bool is_rev_word(const uint16_t instruction) {
	return 0b1011101000 == bits<6, 10>::of(instruction);
}

static bool is_rev16(const uint16_t instruction) {
	return 0b1011101001 == bits<6, 10>::of(instruction);
}

static bool is_revsh(const uint16_t instruction) {
	return 0b1011101011 == bits<6, 10>::of(instruction);
}

static bool is_pop(const uint16_t instruction) {
	return 0b1011110 == bits<9, 7>::of(instruction);
}

static bool is_breakpoint(const uint16_t instruction) {
	return 0b10111110 == bits<8, 8>::of(instruction);
}

static bool is_hints(const uint16_t instruction) {
	return 0b10111111 == bits<8, 8>::of(instruction);
}

bool is_nop(const uint16_t instruction) {
	return 0b1011111100000000 == instruction;
}

static bool is_stm(const uint16_t instruction) {
	return 0b11000 == bits<11, 5>::of(instruction);
}

static bool is_ldm(const uint16_t instruction) {
	return 0b11001 == bits<11, 5>::of(instruction);
}

static bool is_branch(const uint16_t instruction) {
	return 0b1101 == bits<12, 4>::of(instruction) && (bits<9, 3>::of(instruction) != 0b111);
}

static bool is_svc(const uint16_t instruction) {
	return 0b1101 == bits<12, 4>::of(instruction) && 0b1111 == bits<8, 4>::of(instruction);
}

static bool is_unconditional_branch(const uint16_t instruction) {
	return 0b11100 == bits<11, 5>::of(instruction);
}

static bool is_32bit_thumb_encoding(const uint16_t instruction) {
	return 0b111 == bits<13, 3>::of(instruction) && 0b00 != bits<11, 2>::of(instruction);
}

bool is_32bit_thumb_br_misc_ctl(const instruction_pair instr) {
	return is_32bit_thumb_encoding(instr.first()) && 0b10 == bits<11, 2>::of(instr.first()) &&
		   0b1 == bits<15, 1>::of(instr.second());
}

bool is_32bit_thumb_msr(const instruction_pair instr) {
	return is_32bit_thumb_br_misc_ctl(instr) && 0b011100 == bits<5, 6>::of(instr.first()) &&
		   0b000 == (bits<12, 3>::of(instr.second()) & 0b101);
}

bool is_32bit_thumb_mrs(const instruction_pair instr) {
	return is_32bit_thumb_br_misc_ctl(instr) && 0b011111 == bits<5, 6>::of(instr.first()) &&
		   0b000 == (bits<12, 3>::of(instr.second()) & 0b101);
}

bool is_32bit_thumb_udf(const instruction_pair instr) {
	return is_32bit_thumb_br_misc_ctl(instr) && 0b1111111 == bits<4, 7>::of(instr.first()) &&
		   0b000 == (bits<12, 3>::of(instr.second()) & 0b101);
}

bool is_32bit_thumb_misc_ctl(const instruction_pair instr) {
	return is_32bit_thumb_br_misc_ctl(instr) && 0b0111011 == bits<4, 7>::of(instr.first()) &&
		   0b000 == (bits<12, 3>::of(instr.second()) & 0b101);
}

bool is_32bit_thumb_misc_ctl_dsb(const instruction_pair instr) {
	return is_32bit_thumb_misc_ctl(instr) && 0b0100 == bits<4, 4>::of(instr.second());
}

bool is_32bit_thumb_misc_ctl_dmb(const instruction_pair instr) {
	return is_32bit_thumb_misc_ctl(instr) && 0b0101 == bits<4, 4>::of(instr.second());
}

bool is_32bit_thumb_misc_ctl_isb(const instruction_pair instr) {
	return is_32bit_thumb_misc_ctl(instr) && 0b0110 == bits<4, 4>::of(instr.second());
}

static bool is_32bit_thumb_bl(const instruction_pair instr) {
	auto op2 = bits<12, 3>::of(instr.second()) & 0b101;
	return is_32bit_thumb_br_misc_ctl(instr) && 0b101 == op2;
}

static bool is_undefined(const uint16_t instr) {
	return 0b11011110 == bits<8, 8>::of(instr);
}

bool is_undefined32(const instruction_pair instr) {
	return is_32bit_thumb_encoding(instr.first()) &&
		   0b111101111111 == bits<4, 12>::of(instr.first()) &&
		   0b1010 == bits<12, 4>::of(instr.second());
}

} // namespace

namespace micromachine::system {

class instruction_decoder {
public:

	void decode_instruction(const instruction_pair instruction_pair) {
		if(is_32bit_thumb_encoding(instruction_pair.first())) {
			decode_32bit_instruction(instruction_pair);
		} else {
			//_jump_table[instruction_pair.first()]();
			decode_16bit_instruction(instruction_pair.first());
		}
	}

	void decode_32bit_instruction(const instruction_pair instruction_pair) {
		if(is_undefined32(instruction_pair)) {
			dispatch_udfw(udfw(instruction_pair));

			// Branch and miscellaneous control
		} else if(is_32bit_thumb_br_misc_ctl(instruction_pair)) {

			// Check for branch first
			if(is_32bit_thumb_bl(instruction_pair)) {
				dispatch_bl_imm(bl_imm(instruction_pair));
				// then for miscellaneous control instructions first
			} else if(is_32bit_thumb_misc_ctl_dsb(instruction_pair)) {
				dispatch_dsb(dsb(instruction_pair.second()));
			} else if(is_32bit_thumb_misc_ctl_dmb(instruction_pair)) {
				dispatch_dmb(dmb(instruction_pair.second()));
			} else if(is_32bit_thumb_misc_ctl_isb(instruction_pair)) {
				dispatch_isb(isb(instruction_pair.second()));

				// Then UDF 32-bit misc control
			} else if(is_32bit_thumb_udf(instruction_pair)) {
				dispatch_isb(isb(instruction_pair.second()));

				// Then msr and mrs
			} else if(is_32bit_thumb_msr(instruction_pair)) {
				dispatch_msr(msr(instruction_pair));
			} else if(is_32bit_thumb_mrs(instruction_pair)) {
				dispatch_mrs(mrs(instruction_pair));
			} else {
				invalid_instruction(instruction_pair);
			}

		} else {
			invalid_instruction(instruction_pair);
		}
	}

	void decode_16bit_instruction(const uint16_t instr) {

		if(is_nop(instr)) {
			dispatch_nop(nop(instr));
		}

		if(is_lsl_imm(instr)) {
			if(0 == bits<6, 5>::of(instr)) {
				// imm5 is zero, encoding is movs
				dispatch_movs(movs(instr));
			} else {
				dispatch_lsl_imm(lsl_imm(instr));
			}
		} else if(is_lsr_imm(instr)) {
			dispatch_lsr_imm(lsr_imm(instr));
		} else if(is_asr_imm(instr)) {
			dispatch_asr_imm(asr_imm(instr));
		} else if(is_add_reg(instr)) {
			dispatch_add_reg(add_reg(instr));
		} else if(is_subs_reg(instr)) {
			dispatch_subs_reg(subs_reg(instr));
		} else if(is_add_imm(instr)) {
			dispatch_add_imm(add_imm(instr));
		} else if(is_subs_imm(instr)) {
			dispatch_subs_imm(subs_imm(instr));
		} else if(is_mov_imm(instr)) {
			dispatch_mov_imm(mov_imm(instr));
		} else if(is_cmp_imm(instr)) {
			dispatch_cmp_imm(cmp_imm(instr));
		} else if(is_add_imm_t2(instr)) {
			dispatch_add_imm_t2(add_imm_t2(instr));
		} else if(is_subs_imm8(instr)) {
			dispatch_subs_imm8(subs_imm8(instr));

			// Data processing
		} else if(is_and_reg(instr)) {
			dispatch_and_reg(and_reg(instr));
		} else if(is_xor_reg(instr)) {
			dispatch_eor_reg(eor_reg(instr));
		} else if(is_lsl_reg(instr)) {
			dispatch_lsl_reg(lsl_reg(instr));
		} else if(is_lsr_reg(instr)) {
			dispatch_lsr_reg(lsr_reg(instr));
		} else if(is_asr_reg(instr)) {
			dispatch_asr_reg(asr_reg(instr));
		} else if(is_adc_reg(instr)) {
			dispatch_adc(adc(instr));
		} else if(is_sbc(instr)) {
			dispatch_sbc(sbc(instr));
		} else if(is_ror_reg(instr)) {
			dispatch_ror_reg(ror_reg(instr));
		} else if(is_tst_reg(instr)) {
			dispatch_tst_reg(tst_reg(instr));
		} else if(is_rsb_imm(instr)) {
			dispatch_rsb_imm(rsb_imm(instr));
		} else if(is_cmp_reg(instr)) {
			dispatch_cmp_reg(cmp_reg(instr));
		} else if(is_cmn_reg(instr)) {
			dispatch_cmn_reg(cmn_reg(instr));
		} else if(is_orr_reg(instr)) {
			dispatch_orr_reg(orr_reg(instr));
		} else if(is_mul_reg(instr)) {
			dispatch_mul_reg(mul_reg(instr));
		} else if(is_bitclear_reg(instr)) {
			dispatch_bic_reg(bic_reg(instr));
		} else if(is_not_reg(instr)) {
			dispatch_mvn(mvn(instr));

			// Special data instructions and branch and exchange
		} else if(is_add_highreg(instr)) {
			dispatch_add_highreg(add_highreg(instr));
		} else if(is_sdibe_unpredictable_0(instr)) {
			// TODO fault !
		} else if(is_cmp_highreg(instr)) {
			dispatch_cmp_highreg(cmp_highreg(instr));
		} else if(is_mov_highreg(instr)) {
			dispatch_mov_highreg(mov_highreg(instr));
		} else if(is_bx(instr)) {
			dispatch_bx(bx(instr));
		} else if(is_blx(instr)) {
			dispatch_blx(blx(instr));
		} else if(is_ldr_literal(instr)) {
			dispatch_ldr_literal(ldr_literal(instr));

			// load store
		} else if(is_str_reg(instr)) {
			dispatch_str_reg(str_reg(instr));
		} else if(is_strh_reg(instr)) {
			dispatch_strh_reg(strh_reg(instr));
		} else if(is_strb_reg(instr)) {
			dispatch_strb_reg(strb_reg(instr));
		} else if(is_ldrsb_reg(instr)) {
			dispatch_ldrsb_reg(ldrsb_reg(instr));
		} else if(is_ldr_reg(instr)) {
			dispatch_ldr_reg(ldr_reg(instr));
		} else if(is_ldrh_reg(instr)) {
			dispatch_ldrh_reg(ldrh_reg(instr));
		} else if(is_ldrb_reg(instr)) {
			dispatch_ldrb_reg(ldrb_reg(instr));
		} else if(is_ldrsh_reg(instr)) {
			dispatch_ldrsh_reg(ldrsh_reg(instr));
		} else if(is_str_imm(instr)) {
			dispatch_str_imm(str_imm(instr));
		} else if(is_ldr_imm(instr)) {
			dispatch_ldr_imm(ldr_imm(instr));
		} else if(is_strb_imm(instr)) {
			dispatch_strb_imm(strb_imm(instr));
		} else if(is_ldrb_imm(instr)) {
			dispatch_ldrb_imm(ldrb_imm(instr));
		} else if(is_strh_imm(instr)) {
			dispatch_strh_imm(strh_imm(instr));
		} else if(is_ldrh_imm(instr)) {
			dispatch_ldrh_imm(ldrh_imm(instr));
		} else if(is_str_sp_imm(instr)) {
			dispatch_str_sp_imm(str_sp_imm(instr));
		} else if(is_ldr_sp_imm(instr)) {
			dispatch_ldr_sp_imm(ldr_sp_imm(instr));

			// PC relative address
		} else if(is_adr(instr)) {
			dispatch_adr(adr(instr));

			// Generate SP-relative address: ADD (SP plus immediate)
		} else if(is_add_sp_imm(instr)) {
			dispatch_add_sp_imm(add_sp_imm(instr));

			// Miscellaneous 16-bit instructions on page A5-8
		} else if(is_add_sp_imm_t2(instr)) {
			dispatch_add_sp_imm_t2(add_sp_imm_t2(instr));
		} else if(is_sub_sp_imm(instr)) {
			dispatch_sub_sp_imm(sub_sp_imm(instr));
		} else if(is_sxth(instr)) {
			dispatch_sxth(sxth(instr));
		} else if(is_sxtb(instr)) {
			dispatch_sxtb(sxtb(instr));
		} else if(is_uxth(instr)) {
			dispatch_uxth(uxth(instr));
		} else if(is_uxtb(instr)) {
			dispatch_uxtb(uxtb(instr));
		} else if(is_push(instr)) {
			dispatch_push(push(instr));
		} else if(is_cps(instr)) {
			dispatch_cps(cps(instr));
		} else if(is_rev_word(instr)) {
			dispatch_rev_word(rev_word(instr));
		} else if(is_rev16(instr)) {
			dispatch_rev16(rev16(instr));
		} else if(is_revsh(instr)) {
			dispatch_revsh(revsh(instr));
		} else if(is_pop(instr)) {
			dispatch_pop(pop(instr));
		} else if(is_breakpoint(instr)) {
			dispatch_bkpt(bkpt(instr));
		} else if(is_hints(instr)) {
			hint instruction(instr);
			if(0b0000 != instruction.opb()) {
				// undefined
			} else {
				switch((uint8_t)instruction.opa()) {
					case 0b0000: {
						dispatch_nop(nop(instr));
						break;
					}
					case 0b0001: {
						dispatch_yield(yield(instr));
						break;
					}
					case 0b0010: {
						dispatch_wfe(wfe(instr));
						break;
					}
					case 0b0011: {
						dispatch_wfi(wfi(instr));
						break;
					}
					case 0b0100: {
						dispatch_sev(sev(instr));
						break;
					}
					default: {
						// undefined
					}
				}
			}
		} else if(is_stm(instr)) {
			dispatch_stm(stm(instr));
		} else if(is_ldm(instr)) {
			dispatch_ldm(ldm(instr));
		} else if(is_branch(instr)) {
			dispatch_branch(branch(instr));
		} else if(is_svc(instr)) {
			dispatch_svc(svc(instr));
		} else if(is_unconditional_branch(instr)) {
			dispatch_unconditional_branch(unconditional_branch(instr));

		} else if(is_undefined(instr)) {
			dispatch_udf(udf(instr));
		} else {
			// unimplemented instruction
			invalid_instruction(instr);
		}
	}

public:

	void invalid_instruction16(const uint16_t instr) {
		invalid_instruction(instr);
	}

	virtual void invalid_instruction(const uint16_t instr) = 0;

	virtual void invalid_instruction(const instruction_pair instr) = 0;

	virtual void dispatch_nop(const nop instruction) = 0;

	virtual void dispatch_yield(const yield instruction) = 0;

	virtual void dispatch_wfe(const wfe instruction) = 0;

	virtual void dispatch_wfi(const wfi instruction) = 0;

	virtual void dispatch_sev(const sev instruction) = 0;

	virtual void dispatch_lsl_imm(const lsl_imm instruction) = 0;

	virtual void dispatch_lsr_imm(const lsr_imm instruction) = 0;

	virtual void dispatch_asr_imm(const asr_imm instruction) = 0;

	virtual void dispatch_add_reg(const add_reg instruction) = 0;

	virtual void dispatch_subs_reg(const subs_reg instruction) = 0;

	virtual void dispatch_add_imm(const add_imm instruction) = 0;

	virtual void dispatch_subs_imm(const subs_imm instruction) = 0;

	virtual void dispatch_mov_imm(const mov_imm instruction) = 0;

	virtual void dispatch_movs(const movs instruction) = 0;

	virtual void dispatch_cmp_imm(const cmp_imm instruction) = 0;

	virtual void dispatch_add_imm_t2(const add_imm_t2 instruction) = 0;

	virtual void dispatch_subs_imm8(const subs_imm8 instruction) = 0;

	virtual void dispatch_and_reg(const and_reg instruction) = 0;

	virtual void dispatch_eor_reg(const eor_reg instruction) = 0;

	virtual void dispatch_lsl_reg(const lsl_reg instruction) = 0;

	virtual void dispatch_lsr_reg(const lsr_reg instruction) = 0;

	virtual void dispatch_asr_reg(const asr_reg instruction) = 0;

	virtual void dispatch_adc(const adc instruction) = 0;

	virtual void dispatch_sbc(const sbc instruction) = 0;

	virtual void dispatch_ror_reg(const ror_reg instruction) = 0;

	virtual void dispatch_tst_reg(const tst_reg instruction) = 0;

	virtual void dispatch_rsb_imm(const rsb_imm instruction) = 0;

	virtual void dispatch_cmp_reg(const cmp_reg instruction) = 0;

	virtual void dispatch_cmn_reg(const cmn_reg instruction) = 0;

	virtual void dispatch_orr_reg(const orr_reg instruction) = 0;

	virtual void dispatch_mul_reg(const mul_reg instruction) = 0;

	virtual void dispatch_bic_reg(const bic_reg instruction) = 0;

	virtual void dispatch_mvn(const mvn instruction) = 0;

	virtual void dispatch_add_highreg(const add_highreg instruction) = 0;

	virtual void dispatch_cmp_highreg(const cmp_highreg instruction) = 0;

	virtual void dispatch_mov_highreg(const mov_highreg instruction) = 0;

	virtual void dispatch_bx(const bx instruction) = 0;

	virtual void dispatch_blx(const blx instruction) = 0;

	virtual void dispatch_ldr_literal(const ldr_literal instruction) = 0;

	virtual void dispatch_str_reg(const str_reg instruction) = 0;

	virtual void dispatch_strh_reg(const strh_reg instruction) = 0;

	virtual void dispatch_strb_reg(const strb_reg instruction) = 0;

	virtual void dispatch_ldrsb_reg(const ldrsb_reg instruction) = 0;

	virtual void dispatch_ldr_reg(const ldr_reg instruction) = 0;

	virtual void dispatch_ldrh_reg(const ldrh_reg instruction) = 0;

	virtual void dispatch_ldrb_reg(const ldrb_reg instruction) = 0;

	virtual void dispatch_ldrsh_reg(const ldrsh_reg instruction) = 0;

	virtual void dispatch_str_imm(const str_imm instruction) = 0;

	virtual void dispatch_ldr_imm(const ldr_imm instruction) = 0;

	virtual void dispatch_strb_imm(const strb_imm instruction) = 0;

	virtual void dispatch_ldrb_imm(const ldrb_imm instruction) = 0;

	virtual void dispatch_strh_imm(const strh_imm instruction) = 0;

	virtual void dispatch_ldrh_imm(const ldrh_imm instruction) = 0;

	virtual void dispatch_str_sp_imm(const str_sp_imm instruction) = 0;

	virtual void dispatch_ldr_sp_imm(const ldr_sp_imm instruction) = 0;

	virtual void dispatch_adr(const adr instruction) = 0;

	virtual void dispatch_add_sp_imm(const add_sp_imm instruction) = 0;

	virtual void dispatch_add_sp_imm_t2(const add_sp_imm_t2 instruction) = 0;

	virtual void dispatch_sub_sp_imm(const sub_sp_imm instruction) = 0;

	virtual void dispatch_sxth(const sxth instruction) = 0;

	virtual void dispatch_sxtb(const sxtb instruction) = 0;

	virtual void dispatch_uxth(const uxth instruction) = 0;

	virtual void dispatch_uxtb(const uxtb instruction) = 0;

	virtual void dispatch_push(const push instruction) = 0;

	virtual void dispatch_cps(const cps instruction) = 0;

	virtual void dispatch_pop(const pop instruction) = 0;

	virtual void dispatch_bkpt(const bkpt instr) = 0;

	virtual void dispatch_rev_word(const rev_word instruction) = 0;

	virtual void dispatch_rev16(const rev16 instruction) = 0;

	virtual void dispatch_revsh(const revsh instruction) = 0;

	virtual void dispatch_branch(const branch instruction) = 0;

	virtual void dispatch_unconditional_branch(const unconditional_branch instruction) = 0;

	virtual void dispatch_stm(const stm instruction) = 0;

	virtual void dispatch_ldm(const ldm instruction) = 0;

	virtual void dispatch_mrs(const mrs instruction) = 0;

	virtual void dispatch_msr(const msr instruction) = 0;

	virtual void dispatch_bl_imm(const bl_imm instruction) = 0;

	virtual void dispatch_svc(const svc instruction) = 0;

	virtual void dispatch_udf(const udf instr) = 0;

	virtual void dispatch_udfw(const udfw instr) = 0;

	virtual void dispatch_dsb(const dsb instr) = 0;

	virtual void dispatch_dmb(const dmb instr) = 0;

	virtual void dispatch_isb(const isb instr) = 0;
};



} // namespace micromachine::system
