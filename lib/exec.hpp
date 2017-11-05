//
// Created by fla on 17-5-4.
//

#ifndef THUMBEMU_EXEC_HPP
#define THUMBEMU_EXEC_HPP

#include "types.hpp"
#include "instructions.hpp"
#include "alu.hpp"
#include "memory.hpp"
#include "apsr_reg.hpp"
#include "registers.hpp"

static void unpredictable() {
	std::abort();
}



static void exec(const lsl_imm& instruction, registers& regs, apsr_reg& status_reg) {
	word rm = regs.get(instruction.rm);
	const bool carry = alu::lsl_c(rm, instruction.shift_offset(), status_reg.carry_flag());
	status_reg.apply_neg(rm);
	status_reg.apply_zero(rm);
	status_reg.write_carry_flag(carry);
	regs.set(instruction.rd, rm);
}

static void exec(const lsr_imm& instruction, registers& regs, apsr_reg& status_reg) {
	word rm = regs.get(instruction.rm);
	uint32_t applied_offset = instruction.shift_offset();
	const bool carry = alu::lsr_c(rm, applied_offset, status_reg.carry_flag());
	status_reg.apply_neg(rm);
	status_reg.apply_zero(rm);
	status_reg.write_carry_flag(carry);
	regs.set(instruction.rd, rm);
}

static void exec(const asr_imm& instruction, registers& regs, apsr_reg& status_reg) {
	word rm = regs.get(instruction.rm);
	uint32_t applied_offset = instruction.shift_offset();
	const bool carry = alu::asr_c(rm, applied_offset, status_reg.carry_flag());
	status_reg.apply_neg(rm);
	status_reg.apply_zero(rm);
	status_reg.write_carry_flag(carry);
	regs.set(instruction.rd, rm);

}

static void exec(const add_reg& instruction, registers& regs, apsr_reg& status_reg) {
	word rm = regs.get(instruction.rm);
	word rn = regs.get(instruction.rn);
	// left shift of zero is omitted here

	bool carry = false;
	bool overflow = false;
	word result = alu::add_with_carry(rn, rm, false, carry, overflow);

	regs.set(instruction.rd, result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const subs_reg& instruction, registers& regs, apsr_reg& status_reg) {
	word rm = regs.get(instruction.rm);
	word rn = regs.get(instruction.rn);
	// left shift of zero is omitted here

	bool carry = false;
	bool overflow = false;
	word result = alu::add_with_carry(rn, ~rm, true, carry, overflow);

	regs.set(instruction.rd, result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}


static void exec(const add_imm& instruction, registers& regs, apsr_reg& status_reg) {

	word rn 	= regs.get(instruction.rm);
	word imm32 	= instruction.imm3;

	bool carry = false;
	bool overflow = false;
	word result = alu::add_with_carry(rn, imm32, false, carry, overflow);

	regs.set(instruction.rd, result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}


static void exec(const subs_imm& instruction, registers& regs, apsr_reg& status_reg) {
	word rn 	= regs.get(instruction.rm);
	word imm32 	= instruction.imm3;

	bool carry = false;
	bool overflow = false;
	word result = alu::add_with_carry(rn, ~imm32, true, carry, overflow);

	regs.set(instruction.rd, result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const mov_imm& instruction, registers& regs, apsr_reg& status_reg) {
	word imm32 	= instruction.imm8;
	regs.set(instruction.rd, instruction.imm8);
	status_reg.apply_zero(imm32);
	status_reg.apply_neg(imm32);
}

static void exec(const movs& instruction, registers& regs, apsr_reg& status_reg) {
	word val = regs.get(instruction.rm);
	regs.set(instruction.rd, val);
	status_reg.apply_zero(val);
	status_reg.apply_neg(val);
}

static void exec(const cmp_imm& instruction, const registers& regs, apsr_reg& status_reg) {
	// like subtraction, but discard the result
	word imm32 	= instruction.imm8;
	word rn 	= regs.get(instruction.rn);
	bool carry = false;
	bool overflow = false;

	word result = alu::add_with_carry(rn, ~imm32, true, carry, overflow);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const add_imm_t2& instruction, registers& regs, apsr_reg& status_reg) {

	word rn 	= regs.get(instruction.rdn);
	word imm32 	= instruction.imm8;

	bool carry = false;
	bool overflow = false;
	word result = alu::add_with_carry(rn, imm32, false, carry, overflow);

	regs.set(instruction.rdn, result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}


static void exec(const subs_imm8& instruction, registers& regs, apsr_reg& status_reg) {

	word rn 	= regs.get(instruction.rdn);
	word imm32 	= instruction.imm8;

	bool carry = false;
	bool overflow = false;
	word result = alu::add_with_carry(rn, ~imm32, true, carry, overflow);

	regs.set(instruction.rdn, result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const and_reg& instruction, registers& regs, apsr_reg& status_reg) {

	word rd = regs.get(instruction.rdn);
	word rm = regs.get(instruction.rm);

	// left shift of zero is omitted here
	word result = rd & rm;

	regs.set(instruction.rdn, result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const eor_reg& instruction, registers& regs, apsr_reg& status_reg) {

	word rd = regs.get(instruction.rdn);
	word rm = regs.get(instruction.rm);

	// left shift of zero is omitted here
	word result = rd ^ rm;

	regs.set(instruction.rdn, result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const lsl_reg& instruction, registers& regs, apsr_reg& status_reg) {
	word value 			= regs.get(instruction.rdn);
	word shift_offset 	= regs.get(instruction.rm).uint(0, 8);

	bool carry = alu::lsl_c(value, shift_offset, status_reg.carry_flag());

	regs.set(instruction.rdn, value);

	status_reg.write_carry_flag(carry);
	status_reg.apply_neg(value);
	status_reg.apply_zero(value);
}

static void exec(const lsr_reg& instruction, registers& regs, apsr_reg& status_reg) {
	word value 			= regs.get(instruction.rdn);
	word shift_offset 	= regs.get(instruction.rm).uint(0, 8);

	bool carry = alu::lsr_c(value, shift_offset, status_reg.carry_flag());

	regs.set(instruction.rdn, value);

	status_reg.write_carry_flag(carry);
	status_reg.apply_neg(value);
	status_reg.apply_zero(value);
}

static void exec(const asr_reg& instruction, registers& regs, apsr_reg& status_reg) {
	word value 			= regs.get(instruction.rdn);
	word shift_offset 	= regs.get(instruction.rm).uint(0, 8); // max 255

	bool carry = alu::asr_c(value, shift_offset, status_reg.carry_flag());

	regs.set(instruction.rdn, value);

	status_reg.write_carry_flag(carry);
	status_reg.apply_neg(value);
	status_reg.apply_zero(value);
}

static void exec(const adc& instruction, registers& regs, apsr_reg& status_reg) {
	word rn 	= regs.get(instruction.rdn);
	word rm 	= regs.get(instruction.rm);

	// left shift of zero is omitted here
	bool carry = false;
	bool overflow = false;
	word result = alu::add_with_carry(rn, rm, status_reg.carry_flag(), carry, overflow);

	regs.set(instruction.rdn, result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_neg(result);
	status_reg.apply_zero(result);
}

static void exec(const sub_c_reg& instruction, registers& regs, apsr_reg& status_reg) {
	word rn 	= regs.get(instruction.rdn);
	word rm 	= regs.get(instruction.rm);

	// left shift of zero is omitted here
	bool carry = false;
	bool overflow = false;
	word result = alu::add_with_carry(rn, ~rm, status_reg.carry_flag(), carry, overflow);

	regs.set(instruction.rdn, result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_neg(result);
	status_reg.apply_zero(result);
}

static void exec(const ror_reg& instruction, registers& regs, apsr_reg& status_reg) {
	word value 			= regs.get(instruction.rdn);
	word shift_offset 	= regs.get(instruction.rm).uint(0, 8);

	bool carry = alu::ror_c(value, shift_offset, status_reg.carry_flag());

	regs.set(instruction.rdn, value);

	status_reg.write_carry_flag(carry);
	status_reg.apply_neg(value);
	status_reg.apply_zero(value);
}

static void exec(const tst_reg& instruction, const registers& regs, apsr_reg& status_reg) {
	word rn = regs.get(instruction.rn);
	word rm = regs.get(instruction.rm);

	// left shift of zero is omitted here
	word result = rn & rm;

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const rsb_imm& instruction, registers& regs, apsr_reg& status_reg) {
	word rn 	= regs.get(instruction.rn);
	word imm32	= 0; // ARMv6-M only supports a value of 0.

	bool carry = false;
	bool overflow = false;
	word result = alu::add_with_carry(~rn, imm32, true, carry, overflow);

	regs.set(instruction.rd, result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_neg(result);
	status_reg.apply_zero(result);
}

static void exec(const cmp_reg& instruction, const registers& regs, apsr_reg& status_reg) {
	word rn 	= regs.get(instruction.rn);
	word rm 	= regs.get(instruction.rm);

	// left shift of zero is omitted here
	bool carry = false;
	bool overflow = false;
	word result = alu::add_with_carry(rn, ~rm, true, carry, overflow);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_neg(result);
	status_reg.apply_zero(result);
}


static void exec(const cmn_reg& instruction, const registers& regs, apsr_reg& status_reg) {
	word rn 	= regs.get(instruction.rn);
	word rm 	= regs.get(instruction.rm);

	// left shift of zero is omitted here
	bool carry = false;
	bool overflow = false;
	word result = alu::add_with_carry(rn, rm, false, carry, overflow);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_neg(result);
	status_reg.apply_zero(result);
}

static void exec(const lor_reg& instruction, registers& regs, apsr_reg& status_reg) {
	word rd = regs.get(instruction.rdn);
	word rm = regs.get(instruction.rm);

	// left shift of zero is omitted here
	word result = rd | rm;

	regs.set(instruction.rdn, result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const mul_reg& instruction, registers& regs, apsr_reg& status_reg) {

	word rm = regs.get(instruction.rdm);
	word rn = regs.get(instruction.rn);

	// left shift of zero is omitted here
	word result = rm * rn;

	regs.set(instruction.rdm, result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const bic_reg& instruction, registers& regs, apsr_reg& status_reg) {

	word rn = regs.get(instruction.rdn);
	word rm = regs.get(instruction.rm);

	// left shift of zero is omitted here
	word result = rn & ~rm;

	regs.set(instruction.rdn, result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const not_reg& instruction, registers& regs, apsr_reg& status_reg) {

	word rm = regs.get(instruction.rm);

	// left shift of zero is omitted here
	word result = ~rm;

	regs.set(instruction.rd, result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const add_highreg& instruction, registers& regs, apsr_reg& status_reg) {

	//TODO:
	/* if (DN:Rdn) == '1101' || Rm == '1101' then SEE ADD (SP plus register);
	 *	d = UInt(DN:Rdn); n = d; m = UInt(Rm); setflags = FALSE; (shift_t, shift_n) = (SRType_LSL, 0);
	 *	if n == 15 && m == 15 then UNPREDICTABLE;
	 */


	word rn = regs.get(instruction.high_rd());
	word rm = regs.get(instruction.high_rm());
	// left shift of zero is omitted here
	bool carry = false;
	bool overflow = false;
	word result = alu::add_with_carry(rn, rm, false, carry, overflow);


	if(registers::PC == instruction.high_rd()) {
		// TODO: branch thumb
		regs.branch_thumb(result);
	} else {
		regs.set(instruction.high_rd(), result);
	}

	// Do not update flags


}

static void exec(const cmp_highreg& instruction, const registers& regs, apsr_reg& status_reg) {

	if(instruction.high_rn() < 8 && instruction.high_rm() < 8) {
		unpredictable();
	}

	if(registers::PC == instruction.high_rn() || registers::PC == instruction.high_rm()) {
		unpredictable();
	}

	word rn = regs.get(instruction.high_rn());
	word rm = regs.get(instruction.high_rm());

	// left shift of zero is omitted here
	bool carry = false;
	bool overflow = false;
	word result = alu::add_with_carry(rn, ~rm, true, carry, overflow);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_neg(result);
	status_reg.apply_zero(result);

}



static void exec(const mov_highreg& instruction, registers& regs, apsr_reg& status_reg) {

	reg_idx high_rm = instruction.high_rm();
	word result = regs.get(high_rm);

	// if rd is PC, just branch
	if(registers::PC == instruction.high_rd()) {

		regs.branch_thumb(result);

	} else {

		regs.set(instruction.high_rd(), result);

		// dont update flags
	}
}

static void exec(const bx& instruction, registers& regs, apsr_reg& status_reg) {
	if(registers::PC == instruction.rm) {
		unpredictable();
	}
	regs.branch(regs.get(instruction.rm));
}

static void exec(const blx& instruction, registers& regs, apsr_reg& status_reg) {
	if(registers::PC == instruction.rm) {
		unpredictable();
	}

	word jump_addr = regs.get(instruction.rm);
	// pc - 2
	word next_instr_addr = regs.get_pc() - 2; // PC is two instruction ahead because of prefetch

	next_instr_addr.write_bit(0, 1); // force thumb bit for lr
	regs.set_lr(next_instr_addr);

	regs.branch(jump_addr);
}

static void exec(const ldr_literal& instruction, registers& regs, apsr_reg& status_reg, const memory& mem) {
	word offset = instruction.imm32();
	word base 	= regs.get_pc().aligned(4);
	word address = base + offset;
	word value = mem.read32(address);
	regs.set(instruction.rt, value);
}

static void exec(const store_reg_word_reg& instruction, const registers& regs, apsr_reg& status_reg, memory& mem) {

	// left shift of zero is omitted here
	word offset = regs.get(instruction.rm);
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	word value 	=  regs.get(instruction.rt);
	mem.write32(address, value);
}

static void exec(const store_reg_halfword_reg& instruction, const registers& regs, apsr_reg& status_reg, memory& mem) {

	// left shift of zero is omitted here
	word offset = regs.get(instruction.rm);
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	word value 	=  regs.get(instruction.rt);
	mem.write16(address, (uint16_t)value.uint(0, 16));
}

static void exec(const store_reg_byte_reg& instruction, const registers& regs, apsr_reg& status_reg, memory& mem) {

	// left shift of zero is omitted here
	word offset = regs.get(instruction.rm);
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	word value 	= regs.get(instruction.rt);
	mem.write8(address, (uint8_t)value.uint(0, 8));
}

static void exec(const load_reg_sbyte_reg& instruction, registers& regs, apsr_reg& status_reg, const memory& mem) {
	// left shift of zero is omitted here
	word offset = regs.get(instruction.rm);
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	bool ok = false;
	int8_t value = mem.read8(address, ok);
	if(ok) {
		regs.set(instruction.rt, value);
	}
}

static void exec(const ldr_reg& instruction, registers& regs, apsr_reg& status_reg, const memory& mem) {
	// left shift of zero is omitted here
	word offset = regs.get(instruction.rm);
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	bool ok = false;
	word value = mem.read32(address, ok);
	if(ok) {
		regs.set(instruction.rt, value);
	}
}

static void exec(const load_reg_halfword_reg& instruction, registers& regs, apsr_reg& status_reg, const memory& mem) {
	// left shift of zero is omitted here
	word offset = regs.get(instruction.rm);
	word base = regs.get(instruction.rn);
	word address = base + offset;
	bool ok = false;
	halfword value = mem.read16(address, ok);
	if(ok) {
		regs.set(instruction.rt, (uint32_t) value);
	}
}

static void exec(const load_reg_byte_reg& instruction, registers& regs, apsr_reg& status_reg, const memory& mem) {
	// left shift of zero is omitted here
	word offset = regs.get(instruction.rm);
	word base = regs.get(instruction.rn);
	word address = base + offset;
	bool ok = false;
	byte value = mem.read8(address, ok);
	if(ok) {
		regs.set(instruction.rt, (uint32_t) value);
	}
}

static void exec(const load_reg_shalfword_reg& instruction, registers& regs, apsr_reg& status_reg, const memory& mem) {
	// left shift of zero is omitted here
	word offset = regs.get(instruction.rm);
	word base = regs.get(instruction.rn);
	word address = base + offset;
	bool ok = false;
	int16_t value = mem.read16(address, ok);
	if(ok) {
		regs.set(instruction.rt, value);
	}
}


static void exec(const store_word_imm& instruction, const registers& regs, apsr_reg& status_reg, memory& mem) {
	word offset = instruction.imm5 << 2;
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	word value 	=  regs.get(instruction.rt);
	mem.write32(address, value);
}

static void exec(const ldr_imm& instruction, registers& regs, apsr_reg& status_reg, const memory& mem) {
	word offset = instruction.imm32();
	word base = regs.get(instruction.rn);
	word address = base + offset;
	bool ok = false;
	word value = mem.read32(address, ok);
	if (ok) {
		regs.set(instruction.rt, value);
	}
}

static void exec(const store_byte_imm& instruction, const registers& regs, apsr_reg& status_reg, memory& mem) {
	word offset = instruction.imm5;
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	word value 	=  regs.get(instruction.rt);
	mem.write8(address, (uint8_t)value.uint(0, 8));
}

static void exec(const load_byte_imm& instruction, registers& regs, apsr_reg& status_reg, const memory& mem) {
	word offset = instruction.imm5;
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	bool ok = false;
	word value = mem.read8(address, ok);

	if(ok) {
		regs.set(instruction.rt, value);
	}
}

static void exec(const store_halfword_imm& instruction, const registers& regs, apsr_reg& status_reg, memory& mem) {
	word offset = instruction.imm5 << 1;
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	word value 	=  regs.get(instruction.rt);
	mem.write16(address, (uint16_t)value.uint(0, 16));
}

static void exec(const load_halfword_imm& instruction, registers& regs, apsr_reg& status_reg, const memory& mem) {
	word offset = instruction.imm5 << 1;
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	bool ok = false;
	word value = mem.read16(address, ok);
	if(ok) {
		regs.set(instruction.rt, value);
	}
}

static void exec(const store_word_sp_imm& instruction, const registers& regs, apsr_reg& status_reg, memory& mem) {
	word offset = instruction.imm8 << 2;
	word base 	= regs.get_sp(); // SP
	word address = base + offset;
	word value 	=  regs.get(instruction.rt);
	mem.write32(address, value);
}

static void exec(const ldr_imm_sp& instruction, registers& regs, apsr_reg& status_reg, const memory& mem) {
	word offset = instruction.imm8 << 2;
	word base 	= regs.get_sp(); // SP
	word address = base + offset;
	word value = mem.read32(address);
	regs.set(instruction.rt, value);
}

static void exec(const adr& instruction, registers& regs) {
	word offset = instruction.imm32();
	word base 	= regs.get_pc().aligned(4); // PC
	word address = base + offset;
	regs.set(instruction.rd, address);
}


static void exec(const add_sp_imm& instruction, registers& regs) {
	// TODO check if other instructions should go here
	word imm32 = instruction.imm32();
	word sp = regs.get_sp();
	word result = alu::add_with_carry(sp, imm32, false);
	regs.set(instruction.rd, result);
}

static void exec(const add_sp_imm_t2& instruction, registers& regs) {
	// TODO check if other instructions should go here
	word imm32 = instruction.imm32();
	word sp = regs.get_sp();
	word result = alu::add_with_carry(sp, imm32, false);
	regs.set_sp(result);
}

static void exec(const sub_sp_imm& instruction, registers& regs) {
	// TODO check if other instructions should go here
	word imm32 = instruction.imm7 << 2;
	word sp = regs.get_sp();
	word result = alu::add_with_carry(sp, ~imm32, true);
	regs.set_sp(result);
}

static void exec(const sxth& instruction, registers& regs) {
	// Note: ROR(0) is omitted here
	word rm = regs.get(instruction.rm);
	word extended = binops::sign(rm, 16); // sign-extend 16 bit to 32
	regs.set(instruction.rd, extended);
}

static void exec(const sxtb& instruction, registers& regs) {
	// Note: ROR(0) is omitted here
	word rm = regs.get(instruction.rm);
	word extended = binops::sign(rm, 8); // sign-extend 8 bit to 32
	regs.set(instruction.rd, extended);
}

static void exec(const uxth& instruction, registers& regs) {
	// Note: ROR(0) is omitted here
	halfword rm = regs.get(instruction.rm).uint(0, 16);
	word extended(rm); // zero-extend 16 bit to 32
	regs.set(instruction.rd, extended);
}

static void exec(const uxtb& instruction, registers& regs) {
	// Note: ROR(0) is omitted here
	byte rm = regs.get(instruction.rm).uint(0, 8);
	word extended(rm); // zero-extend 8 bit to 32
	regs.set(instruction.rd, extended);
}

static void exec(const push& instruction, registers& regs, memory& mem) {
	precond(instruction.pop_count() > 0, "must push at least one register");
	const size_t stored_size = 4 * instruction.pop_count();
	const word start_address = regs.get_sp() - stored_size;
	size_t count = 0;
	for(reg_idx rid = 0; rid < registers::NUM_REGS-1; rid++) {
		if(instruction.is_set(rid)) {
			mem.write32(start_address + (count*4), regs.get(rid));
			count++;
		}
	}
	regs.set_sp(start_address);
}

static void exec(const pop& instruction, registers& regs, memory& mem) {
	precond(instruction.pop_count() > 0, "must push at least one register");
	const uint32_t frame_start = regs.get_sp(); // sp
	uint32_t base = frame_start;
	const uint32_t stored_size = 4 * instruction.pop_count();
	for (reg_idx rid = 0; rid < 8; rid++) {
		if(instruction.is_set(rid)) {
			regs.set(rid, mem.read32(base));
			base += 4;
		}
	}

	if(instruction.is_set(registers::PC)) {
		regs.branch(mem.read32(base));
	}

	regs.set_sp(frame_start + stored_size);
}

static void exec(const rev_word& instruction, registers& regs) {
	regs.set(instruction.rd, binops::swap(regs.get(instruction.rm)));
}

static void exec(const rev_packed_halfword& instruction, registers& regs) {

	const word rm = regs.get(instruction.rm);

	word res =  (rm.uint(16, 8) << 24) |
				(rm.uint(24, 8) << 16) |
				(rm.uint(0, 8) << 8)   |
				(rm.uint(8, 8) << 0);

	regs.set(instruction.rd, res);
}

static void exec(const rev_packed_signed_halfword& instruction, registers& regs) {

	const word rm = regs.get(instruction.rm);

	word swapped_low16 = (rm.uint(0, 8) << 8) | rm.uint(8, 8);
	word res = binops::sign(swapped_low16, 16);

	regs.set(instruction.rd, res);
}

static void exec(const branch& instruction, registers& regs, apsr_reg& flags) {


	if(!flags.condition_passed(instruction.cond)) {
		return;
	}

	int32_t delta = instruction.offset();
	uint32_t pc = regs.get_pc();
	regs.set_pc(pc + delta);
};

static void exec(const unconditional_branch& instruction, registers& regs, apsr_reg& flags) {
	int32_t delta = instruction.offset();
	uint32_t pc = regs.get_pc();
	regs.set_pc(pc + delta);
};

static void exec(const stm& instruction, registers& regs, memory& mem) {
	uint32_t address = regs.get(instruction.rn);

	for (reg_idx rid = 0; rid < registers::NUM_REGS-1; rid++) {
		if(instruction.is_set(rid)) {
			if(!mem.write32(address, regs.get(rid))) {
				// memory fault. aborting
				return;
			}
			address += 4;
		}
	}

	// write back
	regs.set(instruction.rn, address);
}


static void exec(const ldm& instruction, registers& regs, memory& mem) {
	uint32_t address = regs.get(instruction.rn);

	for (reg_idx rid = 0; rid < 8; rid++) {
		if(instruction.is_set(rid)) {
			// dont modify register if memory access is faulty
			bool mem_access_ok = false;
			uint32_t value = mem.read32(address, mem_access_ok);
			if(mem_access_ok) {
				regs.set(rid, value);
			}
			address += 4;
		}
	}

	// write back unless register was in list
	if(!instruction.is_set(instruction.rn)) {
		regs.set(instruction.rn, address);
	}
}


static void exec(const mrs& instruction, registers& regs, apsr_reg& apsr) {
	// do not keep the value initially present in the register
	word val = 0;

	switch(instruction.sysn.uint(3, 5)) {
		case 0b00000: {
			if(instruction.sysn.bit(0)) {
				val.write_bits(0, 0, regs.interrupt_status_register().exception(), 8);
			}
			if(instruction.sysn.bit(1)) {
				// T-bit reads as zero
				val.write_bit(epsr_reg::THUMB_BIT, 0);
			}
			if(!instruction.sysn.bit(2)) {
				// T-bit reads as zero
				// copy 5 bits from APS
				// TODO: Why 5 and not 4 ???
				val.write_bits(27, 27, regs.xpsr_register(), 5);
			}
		} break;
		case 0b00001: {
			switch(instruction.sysn.uint(0, 3)) {
				case 000: {
					val = regs.sp_register().get_specific_banked_sp(sp_reg::StackType::Main);
				} break;
				case 001: {
					val = regs.sp_register().get_specific_banked_sp(sp_reg::StackType::Process);
				} break;
			}
		} break;
		case 0b00010: {
			switch(instruction.sysn.uint(0, 3)) {
				case 000: {
					// TODO: MRS SpecialRegister::PRIMASK
				} break;
				case 001: {
					val.write_bits(0, 0, regs.control_register(), 2);
				} break;
			}
		} break;
	}
	regs.set(instruction.rd, val);
}

static void exec(const msr& instruction, registers& regs, apsr_reg& apsr) {
	switch(instruction.sysn) {
		case msr::SpecialRegister::APSR:
		case msr::SpecialRegister::IAPSR:
		case msr::SpecialRegister::EAPSR:
		case msr::SpecialRegister::XPSR: {
			// TODO: Write a word.copy_bits
			apsr.copy_bits(regs.get(instruction.rn).uint(0, 5));
		} break;
		case msr::SpecialRegister::MSP: {
			// TODO: Should fail if not in privileged mode
			word sp = regs.get(instruction.rn).uint(2, 30) << 2;
			regs.sp_register().set_specific_banked_sp(sp_reg::StackType::Main, sp);
		} break;
		case msr::SpecialRegister::PSP: {
			// TODO: Should fail if not in privileged mode
			word sp = regs.get(instruction.rn).uint(2, 30);
			regs.sp_register().set_specific_banked_sp(sp_reg::StackType::Process, sp);
		} break;
		case msr::SpecialRegister::PRIMASK: {
			// TODO: MSR SpecialRegister::PRIMASK
			//regs.control_register().n_priv()
		} break;
		case msr::SpecialRegister::CONROL: {
			if(regs.exec_mode_register().is_thread_mode()) {
				word val = regs.get(instruction.rn);
				regs.control_register().set_n_priv(val.bit(0));
				regs.control_register().set_sp_sel(val.bit(1));
			}
		} break;
	}
}

static void exec(const bl_imm& instruction, registers& regs) {
	// pc is 4 bytes ahead, so already poiting to the next instruction
	word next_instr_addr = regs.get_pc();
	next_instr_addr.set_bit(0); // force thumb mode
	regs.set_lr(next_instr_addr);
	int32_t offset = instruction.offset();
	int32_t new_pc = regs.get_pc() + offset;
	regs.set_pc(new_pc);
}

static void exec(const svc& instruction) {
	fprintf(stderr, "SVC %02X\n", instruction.imm8);
}

#endif //THUMBEMU_EXEC_HPP

