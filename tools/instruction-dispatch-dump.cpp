

#include "instruction/instruction_decoder.hpp"

using namespace micromachine::system;

#define print(...) result = #__VA_ARGS__;

class dispatcher : public instruction_decoder {
public:
	std::string result;

private:
	void invalid_instruction(const uint16_t) override {
		print(invalid_instruction16);
	}

	void invalid_instruction(const instruction_pair) override {
		print(invalid_instruction32);
	}

	void dispatch(const nop) override {
		print(nop);
	}
	void dispatch(const yield) override {
		print(yield);
	}
	void dispatch(const wfe) override {
		print(wfe);
	}
	void dispatch(const wfi) override {
		print(wfi);
	}
	void dispatch(const sev) override {
		print(sev);
	}
	void dispatch(const lsl_imm) override {
		print(lsl_imm);
	}
	void dispatch(const lsr_imm) override {
		print(lsr_imm);
	}
	void dispatch(const asr_imm) override {
		print(asr_imm);
	}
	void dispatch(const add_reg) override {
		print(add_reg);
	}
	void dispatch(const subs_reg) override {
		print(subs_reg);
	}
	void dispatch(const add_imm) override {
		print(add_imm);
	}
	void dispatch(const subs_imm) override {
		print(subs_imm);
	}
	void dispatch(const mov_imm) override {
		print(mov_imm);
	}
	void dispatch(const movs) override {
		print(movs);
	}
	void dispatch(const cmp_imm) override {
		print(cmp_imm);
	}
	void dispatch(const add_imm_t2) override {
		print(add_imm_t2);
	}
	void dispatch(const subs_imm8) override {
		print(subs_imm8);
	}
	void dispatch(const and_reg) override {
		print(and_reg);
	}
	void dispatch(const eor_reg) override {
		print(eor_reg);
	}
	void dispatch(const lsl_reg) override {
		print(lsl_reg);
	}
	void dispatch(const lsr_reg) override {
		print(lsr_reg);
	}
	void dispatch(const asr_reg) override {
		print(asr_reg);
	}
	void dispatch(const adc) override {
		print(adc);
	}
	void dispatch(const sbc) override {
		print(sbc);
	}
	void dispatch(const ror_reg) override {
		print(ror_reg);
	}
	void dispatch(const tst_reg) override {
		print(tst_reg);
	}
	void dispatch(const rsb_imm) override {
		print(rsb_imm);
	}
	void dispatch(const cmp_reg) override {
		print(cmp_reg);
	}
	void dispatch(const cmn_reg) override {
		print(cmn_reg);
	}
	void dispatch(const orr_reg) override {
		print(orr_reg);
	}
	void dispatch(const mul_reg) override {
		print(mul_reg);
	}
	void dispatch(const bic_reg) override {
		print(bic_reg);
	}
	void dispatch(const mvn) override {
		print(mvn);
	}
	void dispatch(const add_highreg) override {
		print(add_highreg);
	}
	void dispatch(const cmp_highreg) override {
		print(cmp_highreg);
	}
	void dispatch(const mov_highreg) override {
		print(mov_highreg);
	}
	void dispatch(const bx) override {
		print(bx);
	}
	void dispatch(const blx) override {
		print(blx);
	}
	void dispatch(const ldr_literal) override {
		print(ldr_literal);
	}
	void dispatch(const str_reg) override {
		print(str_reg);
	}
	void dispatch(const strh_reg) override {
		print(strh_reg);
	}
	void dispatch(const strb_reg) override {
		print(strb_reg);
	}
	void dispatch(const ldrsb_reg) override {
		print(ldrsb_reg);
	}
	void dispatch(const ldr_reg) override {
		print(ldr_reg);
	}
	void dispatch(const ldrh_reg) override {
		print(ldrh_reg);
	}
	void dispatch(const ldrb_reg) override {
		print(ldrb_reg);
	}
	void dispatch(const ldrsh_reg) override {
		print(ldrsh_reg);
	}
	void dispatch(const str_imm) override {
		print(str_imm);
	}
	void dispatch(const ldr_imm) override {
		print(ldr_imm);
	}
	void dispatch(const strb_imm) override {
		print(strb_imm);
	}
	void dispatch(const ldrb_imm) override {
		print(ldrb_imm);
	}
	void dispatch(const strh_imm) override {
		print(strh_imm);
	}
	void dispatch(const ldrh_imm) override {
		print(ldrh_imm);
	}
	void dispatch(const str_sp_imm) override {
		print(str_sp_imm);
	}
	void dispatch(const ldr_sp_imm) override {
		print(ldr_sp_imm);
	}
	void dispatch(const adr) override {
		print(adr);
	}
	void dispatch(const add_sp_imm) override {
		print(add_sp_imm);
	}
	void dispatch(const add_sp_imm_t2) override {
		print(add_sp_imm_t2);
	}
	void dispatch(const sub_sp_imm) override {
		print(sub_sp_imm);
	}
	void dispatch(const sxth) override {
		print(sxth);
	}
	void dispatch(const sxtb) override {
		print(sxtb);
	}
	void dispatch(const uxth) override {
		print(uxth);
	}
	void dispatch(const uxtb) override {
		print(uxtb);
	}
	void dispatch(const push) override {
		print(push);
	}
	void dispatch(const cps) override {
		print(cps);
	}
	void dispatch(const pop) override {
		print(pop);
	}
	void dispatch(const bkpt) override {
		print(bkpt);
	}
	void dispatch(const rev_word) override {
		print(rev_word);
	}
	void dispatch(const rev16) override {
		print(rev16);
	}
	void dispatch(const revsh) override {
		print(revsh);
	}
	void dispatch(const branch) override {
		print(branch);
	}
	void dispatch(const unconditional_branch) override {
		print(unconditional_branch);
	}
	void dispatch(const stm) override {
		print(stm);
	}
	void dispatch(const ldm) override {
		print(ldm);
	}
	void dispatch(const mrs) override {
		print(mrs);
	}
	void dispatch(const msr) override {
		print(msr);
	}
	void dispatch(const bl_imm) override {
		print(bl_imm);
	}
	void dispatch(const svc) override {
		print(svc);
	}
	void dispatch(const dmb) override {
		print(dmb);
	}
	void dispatch(const dsb) override {
		print(dsb);
	}
	void dispatch(const isb) override {
		print(isb);
	}
	void dispatch(const udf) override {
		print(udf);
	}
	void dispatch(const udfw) override {
		print(udfw);
	}
};


int main(int argc, char** argv) {

	for(size_t i = 0; i < 0x10000; ++i) {
		dispatcher disp;
		disp.result.clear();
		disp.decode_16bit_instruction((uint16_t)i);

		if(disp.result == "") {
			disp.result = "invalid_instruction16";
		}

		if(disp.result == "invalid_instruction16") {
			printf("_jump_table[0x%04xU] = [this](){ invalid_instruction16((uint16_t)0x%04xU); };\n",(uint16_t)i,(uint16_t)i);
		} else if(disp.result == "invalid_instruction32") {
			std::abort();
		} else {
			printf("_jump_table[0x%04xU] = [this](){ dispatch(%s((uint16_t)0x%04xU)); };\n",
				   (uint16_t)i,disp.result.c_str(), (uint16_t)i);
		}
	}

	return EXIT_SUCCESS;
}
