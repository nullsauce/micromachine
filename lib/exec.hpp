//
// Created by fla on 17-5-4.
//

#ifndef THUMBEMU_EXEC_HPP
#define THUMBEMU_EXEC_HPP

#include "types.hpp"
#include "instructions.hpp"
#include "alu.hpp"
#include "memory.hpp"
#include "apsr_register.hpp"
#include "register_set.hpp"
//#include "regset.hpp"
//#include "memory.hpp"

static void unpredictable() {
	std::abort();
}

/*
static
void exec_ldr(reg_idx rd, reg_idx rs, reg_idx ro, regset& regs, const memory& mem) {
	int32_t address = regs[rs] + regs[ro];
	regs[rd] = mem.read32(address);
}*/


void exec(const lsl_imm& instruction, register_set& regs, apsr_register& status_reg) {
	word rm = regs.get(instruction.rm);
	const bool carry = alu::lsl_c(rm, instruction.shift_offset(), status_reg.carry_flag());
	status_reg.apply_neg(rm);
	status_reg.apply_zero(rm);
	status_reg.write_carry_flag(carry);
	regs.set(instruction.rd, rm);
}

void exec(const lsr_imm& instruction, register_set& regs, apsr_register& status_reg) {
	word rm = regs.get(instruction.rm);
	const bool carry = alu::lsr_c(rm, instruction.shift_offset(), status_reg.carry_flag());
	status_reg.apply_neg(rm);
	status_reg.apply_zero(rm);
	status_reg.write_carry_flag(carry);
	regs.set(instruction.rd, rm);
}

void exec(const asr_imm& instruction, register_set& regs, apsr_register& status_reg) {
	word rm = regs.get(instruction.rm);
	const bool carry = alu::asr_c(rm, instruction.shift_offset(), status_reg.carry_flag());
	status_reg.apply_neg(rm);
	status_reg.apply_zero(rm);
	status_reg.write_carry_flag(carry);
	regs.set(instruction.rd, rm);
}

void exec(const adds_reg& instruction, register_set& regs, apsr_register& status_reg) {
	word rm = regs.get(instruction.rm);
	word rn = regs.get(instruction.rm);
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

void exec(const subs_reg& instruction, register_set& regs, apsr_register& status_reg) {
	word rm = regs.get(instruction.rm);
	word rn = regs.get(instruction.rm);
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


void exec(const adds_imm& instruction, register_set& regs, apsr_register& status_reg) {

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


void exec(const subs_imm& instruction, register_set& regs, apsr_register& status_reg) {
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

void exec(const mov_imm& instruction, register_set& regs, apsr_register& status_reg) {
	word imm32 	= instruction.imm8;
	regs.set(instruction.rd, instruction.imm8);
	status_reg.apply_zero(imm32);
	status_reg.apply_neg(imm32);
}

void exec(const cmp_imm& instruction, const register_set& regs, apsr_register& status_reg) {
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

void exec(const adds_imm8& instruction, register_set& regs, apsr_register& status_reg) {

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


void exec(const subs_imm8& instruction, register_set& regs, apsr_register& status_reg) {

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

void exec(const and_reg& instruction, register_set& regs, apsr_register& status_reg) {

	word rd = regs.get(instruction.rdn);
	word rm = regs.get(instruction.rm);

	// left shift of zero is omitted here
	word result = rd & rm;

	regs.set(instruction.rdn, result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

void exec(const xor_reg& instruction, register_set& regs, apsr_register& status_reg) {

	word rd = regs.get(instruction.rdn);
	word rm = regs.get(instruction.rm);

	// left shift of zero is omitted here
	word result = rd ^ rm;

	regs.set(instruction.rdn, result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

void exec(const lsl_reg& instruction, register_set& regs, apsr_register& status_reg) {
	word value 			= regs.get(instruction.rdn);
	word shift_offset 	= regs.get(instruction.rm).uint(0, 7); // max 127

	bool carry = alu::lsl_c(value, shift_offset, status_reg.carry_flag());

	regs.set(instruction.rdn, value);

	status_reg.write_carry_flag(carry);
	status_reg.apply_neg(value);
	status_reg.apply_zero(value);
}

void exec(const lsr_reg& instruction, register_set& regs, apsr_register& status_reg) {
	word value 			= regs.get(instruction.rdn);
	word shift_offset 	= regs.get(instruction.rm).uint(0, 7); // max 127

	bool carry = alu::lsr_c(value, shift_offset, status_reg.carry_flag());

	regs.set(instruction.rdn, value);

	status_reg.write_carry_flag(carry);
	status_reg.apply_neg(value);
	status_reg.apply_zero(value);
}

void exec(const asr_reg& instruction, register_set& regs, apsr_register& status_reg) {
	word value 			= regs.get(instruction.rdn);
	word shift_offset 	= regs.get(instruction.rm).uint(0, 7); // max 127

	bool carry = alu::asr_c(value, shift_offset, status_reg.carry_flag());

	regs.set(instruction.rdn, value);

	status_reg.write_carry_flag(carry);
	status_reg.apply_neg(value);
	status_reg.apply_zero(value);
}

void exec(const add_c_reg& instruction, register_set& regs, apsr_register& status_reg) {
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

void exec(const sub_c_reg& instruction, register_set& regs, apsr_register& status_reg) {
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

void exec(const ror_reg& instruction, register_set& regs, apsr_register& status_reg) {
	word value 			= regs.get(instruction.rdn);
	word shift_offset 	= regs.get(instruction.rm).uint(0, 7); // max 127

	bool carry = alu::ror_c(value, shift_offset, status_reg.carry_flag());

	regs.set(instruction.rdn, value);

	status_reg.write_carry_flag(carry);
	status_reg.apply_neg(value);
	status_reg.apply_zero(value);
}

void exec(const tst_reg& instruction, const register_set& regs, apsr_register& status_reg) {
	word rn = regs.get(instruction.rn);
	word rm = regs.get(instruction.rm);

	// left shift of zero is omitted here
	word result = rn & rm;

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

void exec(const rsb_imm& instruction, register_set& regs, apsr_register& status_reg) {
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

void exec(const cmp_reg& instruction, const register_set& regs, apsr_register& status_reg) {
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


void exec(const cmpn_reg& instruction, const register_set& regs, apsr_register& status_reg) {
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

void exec(const lor_reg& instruction, register_set& regs, apsr_register& status_reg) {
	word rd = regs.get(instruction.rdn);
	word rm = regs.get(instruction.rm);

	// left shift of zero is omitted here
	word result = rd | rm;

	regs.set(instruction.rdn, result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

void exec(const mul_reg& instruction, register_set& regs, apsr_register& status_reg) {

	word rm = regs.get(instruction.rdm);
	word rn = regs.get(instruction.rn);

	// left shift of zero is omitted here
	word result = rm * rn;

	regs.set(instruction.rdm, result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

void exec(const bic_reg& instruction, register_set& regs, apsr_register& status_reg) {

	word rn = regs.get(instruction.rdn);
	word rm = regs.get(instruction.rm);

	// left shift of zero is omitted here
	word result = rn & ~rm;

	regs.set(instruction.rdn, result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

void exec(const not_reg& instruction, register_set& regs, apsr_register& status_reg) {

	word rm = regs.get(instruction.rm);

	// left shift of zero is omitted here
	word result = ~rm;

	regs.set(instruction.rd, result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

void exec(const add_highreg& instruction, register_set& regs, apsr_register& status_reg) {

	//TODO:
	/* if (DN:Rdn) == '1101' || Rm == '1101' then SEE ADD (SP plus register);
	 *	d = UInt(DN:Rdn); n = d; m = UInt(Rm); setflags = FALSE; (shift_t, shift_n) = (SRType_LSL, 0);
	 *	if n == 15 && m == 15 then UNPREDICTABLE;
	 */

	// if rd is PC, just branch
	if(15 == instruction.high_rd()) {

		// TODO: branch
		assert(false);

	} else {
		word rn = regs.get(instruction.high_rd());
		word rm = regs.get(instruction.high_rm());
		// left shift of zero is omitted here
		bool carry = false;
		bool overflow = false;
		word result = alu::add_with_carry(rn, rm, false, carry, overflow);
		regs.set(instruction.high_rd(), result);

		// Do not update flags
	}

}

void exec(const cmp_highreg& instruction, const register_set& regs, apsr_register& status_reg) {

	if(instruction.high_rn() < 8 && instruction.high_rm() < 8) {
		unpredictable();
	}

	if(15 == instruction.high_rn() || 15 == instruction.high_rm()) {
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



void exec(const mov_highreg& instruction, register_set& regs, apsr_register& status_reg) {

	// if rd is PC, just branch
	if(15 == instruction.high_rd()) {

		// TODO: branch
		assert(false);

	} else {

		reg_idx high_rm = instruction.high_rm();
		word result = regs.get(high_rm);
		regs.set(instruction.high_rd(), result);

		// dont update flags
	}
}

void exec(const bx& instruction, register_set& regs, apsr_register& status_reg) {
	if(15 == instruction.rm) {
		unpredictable();
	}
	regs.setPC(regs.get(instruction.rm));
}

void exec(const blx& instruction, register_set& regs, apsr_register& status_reg) {
	if(15 == instruction.rm) {
		unpredictable();
	}

	// pc - 2
	word next_instr_addr = regs.getPC() - 2; // PC is two instruction ahead because of prefetch

	next_instr_addr.write_bit(0, 1); // thumb bit
	regs.setPC(next_instr_addr);
}

void exec(const load_literal& instruction, register_set& regs, apsr_register& status_reg, const memory& mem) {
	word offset = instruction.imm8 << 2;
	word base 	= regs.getPC().aligned(4);
	word address = base + offset;
	word value = mem.read32(address);
	regs.set(instruction.rt, value);
}

void exec(const store_reg_word_reg& instruction, const register_set& regs, apsr_register& status_reg, memory& mem) {

	// left shift of zero is omitted here
	word offset = regs.get(instruction.rm);
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	word value 	=  regs.get(instruction.rt);
	mem.write32(address, value);
}

void exec(const store_reg_halfword_reg& instruction, const register_set& regs, apsr_register& status_reg, memory& mem) {

	// left shift of zero is omitted here
	word offset = regs.get(instruction.rm);
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	word value 	=  regs.get(instruction.rt);
	mem.write16(address, (uint16_t)value.uint(0, 16));
}

void exec(const store_reg_byte_reg& instruction, const register_set& regs, apsr_register& status_reg, memory& mem) {

	// left shift of zero is omitted here
	word offset = regs.get(instruction.rm);
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	word value 	= regs.get(instruction.rt);
	mem.write8(address, (uint8_t)value.uint(0, 8));
}

void exec(const load_reg_sbyte_reg& instruction, register_set& regs, apsr_register& status_reg, const memory& mem) {
	// left shift of zero is omitted here
	word offset = regs.get(instruction.rm);
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	word value = mem.read8(address);
	binops::sign(value, 8);
	regs.set(instruction.rt, value);

}

void exec(const load_reg_word_reg& instruction, register_set& regs, apsr_register& status_reg, const memory& mem) {
	// left shift of zero is omitted here
	word offset = regs.get(instruction.rm);
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	word value = mem.read32(address);
	regs.set(instruction.rt, value);
}

void exec(const load_reg_halfword_reg& instruction, register_set& regs, apsr_register& status_reg, const memory& mem) {
	// left shift of zero is omitted here
	word offset = regs.get(instruction.rm);
	word base = regs.get(instruction.rn);
	word address = base + offset;
	halfword value = mem.read16(address);
	regs.set(instruction.rt, (uint32_t)value);
}

void exec(const load_reg_byte_reg& instruction, register_set& regs, apsr_register& status_reg, const memory& mem) {
	// left shift of zero is omitted here
	word offset = regs.get(instruction.rm);
	word base = regs.get(instruction.rn);
	word address = base + offset;
	byte value = mem.read8(address);
	regs.set(instruction.rt, (uint32_t)value);
}

void exec(const load_reg_shalfword_reg& instruction, register_set& regs, apsr_register& status_reg, const memory& mem) {
	// left shift of zero is omitted here
	word offset = regs.get(instruction.rm);
	word base = regs.get(instruction.rn);
	word address = base + offset;
	word value = mem.read16(address);
	binops::sign(value, 16);
	regs.set(instruction.rt, value);
}


void exec(const store_word_imm& instruction, const register_set& regs, apsr_register& status_reg, memory& mem) {
	word offset = instruction.imm5 << 2;
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	word value 	=  regs.get(instruction.rt);
	mem.write32(address, value);
}

void exec(const load_word_imm& instruction, register_set& regs, apsr_register& status_reg, const memory& mem) {
	word offset = instruction.imm5 << 2;
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	word value = mem.read32(address);
	regs.set(instruction.rt, value);
}

void exec(const store_byte_imm& instruction, const register_set& regs, apsr_register& status_reg, memory& mem) {
	word offset = instruction.imm5;
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	word value 	=  regs.get(instruction.rt);
	mem.write8(address, (uint8_t)value.uint(0, 8));
}

void exec(const load_byte_imm& instruction, register_set& regs, apsr_register& status_reg, const memory& mem) {
	word offset = instruction.imm5;
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	word value = mem.read8(address);
	regs.set(instruction.rt, value);
}

void exec(const store_halfword_imm& instruction, const register_set& regs, apsr_register& status_reg, memory& mem) {
	word offset = instruction.imm5 << 1;
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	word value 	=  regs.get(instruction.rt);
	mem.write16(address, (uint16_t)value.uint(0, 16));
}

void exec(const load_halfword_imm& instruction, register_set& regs, apsr_register& status_reg, const memory& mem) {
	word offset = instruction.imm5 << 1;
	word base 	= regs.get(instruction.rn);
	word address = base + offset;
	word value = mem.read16(address);
	regs.set(instruction.rt, value);
}

void exec(const store_word_sp_imm& instruction, const register_set& regs, apsr_register& status_reg, memory& mem) {
	word offset = instruction.imm8 << 2;
	word base 	= regs.getSP(); // SP
	word address = base + offset;
	word value 	=  regs.get(instruction.rt);
	mem.write32(address, value);
}

void exec(const load_word_sp_imm& instruction, register_set& regs, apsr_register& status_reg, const memory& mem) {
	word offset = instruction.imm8 << 2;
	word base 	= regs.getSP(); // SP
	word address = base + offset;
	word value = mem.read32(address);
	regs.set(instruction.rt, value);
}

void exec(const adr& instruction, register_set& regs) {
	word offset = instruction.imm8 << 2;
	word base 	= regs.getPC().aligned(4); // PC
	word address = base + offset;
	regs.set(instruction.rd, address);
}


void exec(const add_sp_imm& instruction, register_set& regs) {
	// TODO check if other instructions should go here
	word imm32 = instruction.imm8 << 2;
	word sp = regs.getSP();
	word result = alu::add_with_carry(sp, imm32, false);
	regs.set(instruction.rd, result);
}

void exec(const add_sp_imm_t2& instruction, register_set& regs) {
	// TODO check if other instructions should go here
	word imm32 = instruction.imm7 << 2;
	word sp = regs.getSP();
	word result = alu::add_with_carry(sp, imm32, false);
	regs.setSP(result);
}

void exec(const sub_sp_imm& instruction, register_set& regs) {
	// TODO check if other instructions should go here
	word imm32 = instruction.imm7 << 2;
	word sp = regs.getSP();
	word result = alu::add_with_carry(sp, ~imm32, true);
	regs.setSP(result);
}

void exec(const sxth& instruction, register_set& regs) {
	// Note: ROR(0) is omitted here
	word rm = regs.get(instruction.rm);
	word extended = binops::sign(rm, 16); // sign-extend 16 bit to 32
	regs.set(instruction.rd, extended);
}

void exec(const sxtb& instruction, register_set& regs) {
	// Note: ROR(0) is omitted here
	word rm = regs.get(instruction.rm);
	word extended = binops::sign(rm, 8); // sign-extend 8 bit to 32
	regs.set(instruction.rd, extended);
}

void exec(const uxth& instruction, register_set& regs) {
	// Note: ROR(0) is omitted here
	word rm = regs.get(instruction.rm);
	word extended = rm; // zero-extend 16 bit to 32
	regs.set(instruction.rd, extended);
}

void exec(const uxtb& instruction, register_set& regs) {
	// Note: ROR(0) is omitted here
	word rm = regs.get(instruction.rm);
	word extended = rm; // zero-extend 8 bit to 32
	regs.set(instruction.rd, extended);
}

void exec(const push& instruction, register_set& regs, memory& mem) {
	precond(instruction.pop_count() > 0, "must push at least one register");
	const size_t stored_size = 4 * instruction.pop_count();
	const word start_address = regs.getSP() - stored_size;
	size_t count = 0;
	for(reg_idx rid = 0; rid < 15; rid++) {
		if(instruction.is_set(rid)) {
			mem.write32(start_address + (count*4), regs.get(rid));
			count++;
		}
	}
	regs.setSP(start_address);
}

void exec(const pop& instruction, register_set& regs, memory& mem) {
	precond(instruction.pop_count() > 0, "must push at least one register");
	uint32_t base = regs.getSP(); // sp
	const uint32_t stored_size = 4 * instruction.pop_count();
	for (reg_idx rid = 0; rid < 8; rid++) {
		if(instruction.is_set(rid)) {
			regs.set(rid, mem.read32(base));
			base += 4;
		}
	}
	if(instruction.is_set(15)) {// pc
		// TODO: Branch
		regs.setPC(mem.read32(base));
	}

	regs.setSP(base + stored_size);
}

void exec(const rev_word& instruction, register_set& regs) {
	regs.set(instruction.rd, binops::swap(regs.get(instruction.rm)));
}

void exec(const rev_packed_halfword& instruction, register_set& regs) {

	const word rm = regs.get(instruction.rm);

	word res =  (rm.uint(16, 8) << 24) |
				(rm.uint(24, 8) << 16) |
				(rm.uint(0, 8) << 8)   |
				(rm.uint(8, 8) << 0);

	regs.set(instruction.rd, res);
}

void exec(const rev_packed_signed_halfword& instruction, register_set& regs) {

	const word rm = regs.get(instruction.rm);

	word swapped_low16 = (rm.uint(0, 8) << 8) | rm.uint(8, 8);
	word res = binops::sign(swapped_low16, 16);

	regs.set(instruction.rd, res);
}

void exec(const branch& instruction, register_set& regs, apsr_register& flags) {


	if(!flags.condition_passed(instruction.cond)) {
		return;
	}

	uint32_t offset = (instruction.imm8 << 1);
	int32_t delta = binops::sign<int32_t>(offset, 9);

	uint32_t pc = regs.getPC();
	regs.setPC(pc + delta);
};

void exec(const unconditional_branch& instruction, register_set& regs, apsr_register& flags) {

	uint32_t offset = (instruction.imm11 << 1);
	int32_t delta = binops::sign<int32_t>(offset, 12);

	uint32_t pc = regs.getPC();
	regs.setPC(pc + delta);
};

void exec(const stm& instruction, register_set& regs, memory& mem) {
	uint32_t address = regs.get(instruction.rn);

	for (reg_idx rid = 0; rid < 15; rid++) {
		if(instruction.is_set(rid)) {
			mem.write32(address, regs.get(rid));
			address += 4;
		}
	}

	// write back
	regs.set(instruction.rn, address);
}


void exec(const ldm& instruction, register_set& regs, memory& mem) {
	uint32_t address = regs.get(instruction.rn);

	for (reg_idx rid = 0; rid < 8; rid++) {
		if(instruction.is_set(rid)) {
			regs.set(rid, mem.read32(address));
			address += 4;
		}
	}

	// write back unless register was in list
	if(!instruction.is_set(instruction.rn)) {
		regs.set(instruction.rn, address);
	}
}

#endif //THUMBEMU_EXEC_HPP
