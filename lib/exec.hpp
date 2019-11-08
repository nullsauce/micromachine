//
// Created by fla on 17-5-4.
//

#ifndef THUMBEMU_EXEC_HPP
#define THUMBEMU_EXEC_HPP

#include "types.hpp"
#include "instructions.hpp"
#include "alu.hpp"
#include "memory.hpp"
#include "registers/apsr_reg.hpp"
#include "registers/registers.hpp"

static void unpredictable() {
	std::abort();
}



static void exec(const lsl_imm instruction, registers& regs, apsr_reg& status_reg) {
	uint32_t rm = regs.get(instruction.rm());
	const bool carry = alu::lsl_c(rm, instruction.shift_offset(), status_reg.carry_flag());
	status_reg.apply_neg(rm);
	status_reg.apply_zero(rm);
	status_reg.write_carry_flag(carry);
	regs.set(instruction.rd(), rm);
}

static void exec(const lsr_imm instruction, registers& regs, apsr_reg& status_reg) {
	uint32_t rm = regs.get(instruction.rm());
	uint32_t applied_offset = instruction.shift_offset();
	if(applied_offset == 0) applied_offset = 32;
	const bool carry = alu::lsr_c(rm, applied_offset, status_reg.carry_flag());
	status_reg.apply_neg(rm);
	status_reg.apply_zero(rm);
	status_reg.write_carry_flag(carry);
	regs.set(instruction.rd(), rm);
}

static void exec(const asr_imm instruction, registers& regs, apsr_reg& status_reg) {
	uint32_t rm = regs.get(instruction.rm());
	uint32_t applied_offset = instruction.shift_offset();
	if(applied_offset == 0) applied_offset = 32;
	const bool carry = alu::asr_c(rm, applied_offset, status_reg.carry_flag());
	status_reg.apply_neg(rm);
	status_reg.apply_zero(rm);
	status_reg.write_carry_flag(carry);
	regs.set(instruction.rd(), rm);

}

static void exec(const add_reg instruction, registers& regs, apsr_reg& status_reg) {
	uint32_t rm = regs.get(instruction.rm());
	uint32_t rn = regs.get(instruction.rn());
	// left shift of zero is omitted here

	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, rm, false, carry, overflow);

	regs.set(instruction.rd(), result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const subs_reg instruction, registers& regs, apsr_reg& status_reg) {
	uint32_t rm = regs.get(instruction.rm());
	uint32_t rn = regs.get(instruction.rn());
	// left shift of zero is omitted here

	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, ~rm, true, carry, overflow);

	regs.set(instruction.rd(), result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}


static void exec(const add_imm instruction, registers& regs, apsr_reg& status_reg) {

	uint32_t rn 	= regs.get(instruction.rn());
	uint32_t imm32 	= instruction.imm3();

	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, imm32, false, carry, overflow);

	regs.set(instruction.rd(), result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}


static void exec(const subs_imm instruction, registers& regs, apsr_reg& status_reg) {
	uint32_t rn 	= regs.get(instruction.rn());
	uint32_t imm32 	= instruction.imm3();

	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, ~imm32, true, carry, overflow);

	regs.set(instruction.rd(), result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const mov_imm instruction, registers& regs, apsr_reg& status_reg) {
	uint32_t imm32 	= instruction.imm8();
	regs.set(instruction.rd(), instruction.imm8());
	status_reg.apply_zero(imm32);
	status_reg.apply_neg(imm32);
}

static void exec(const movs instruction, registers& regs, apsr_reg& status_reg) {
	uint32_t val = regs.get(instruction.rm());
	regs.set(instruction.rd(), val);
	status_reg.apply_zero(val);
	status_reg.apply_neg(val);
}

static void exec(const cmp_imm instruction, const registers& regs, apsr_reg& status_reg) {
	// like subtraction, but discard the result
	uint32_t imm32 	= instruction.imm8();
	uint32_t rn 	= regs.get(instruction.rn());
	bool carry = false;
	bool overflow = false;

	uint32_t result = alu::add_with_carry(rn, ~imm32, true, carry, overflow);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const add_imm_t2 instruction, registers& regs, apsr_reg& status_reg) {

	uint32_t rn 	= regs.get(instruction.rdn());
	uint32_t imm32 	= instruction.imm8();

	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, imm32, false, carry, overflow);

	regs.set(instruction.rdn(), result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}


static void exec(const subs_imm8 instruction, registers& regs, apsr_reg& status_reg) {

	uint32_t rn 	= regs.get(instruction.rdn());
	uint32_t imm32 	= instruction.imm8();

	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, ~imm32, true, carry, overflow);

	regs.set(instruction.rdn(), result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const and_reg instruction, registers& regs, apsr_reg& status_reg) {

	uint32_t rd = regs.get(instruction.rdn());
	uint32_t rm = regs.get(instruction.rm());

	// left shift of zero is omitted here
	uint32_t result = rd & rm;

	regs.set(instruction.rdn(), result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const eor_reg instruction, registers& regs, apsr_reg& status_reg) {

	uint32_t rd = regs.get(instruction.rdn());
	uint32_t rm = regs.get(instruction.rm());

	// left shift of zero is omitted here
	uint32_t result = rd ^ rm;

	regs.set(instruction.rdn(), result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const lsl_reg instruction, registers& regs, apsr_reg& status_reg) {
	uint32_t value 			= regs.get(instruction.rdn());
	uint32_t shift_offset 	= bits<0,8>::of(regs.get(instruction.rm()));

	bool carry = alu::lsl_c(value, shift_offset, status_reg.carry_flag());

	regs.set(instruction.rdn(), value);

	status_reg.write_carry_flag(carry);
	status_reg.apply_neg(value);
	status_reg.apply_zero(value);
}

static void exec(const lsr_reg instruction, registers& regs, apsr_reg& status_reg) {
	uint32_t value 			= regs.get(instruction.rdn());
	uint32_t shift_offset 	= bits<0,8>::of(regs.get(instruction.rm()));

	bool carry = alu::lsr_c(value, shift_offset, status_reg.carry_flag());

	regs.set(instruction.rdn(), value);

	status_reg.write_carry_flag(carry);
	status_reg.apply_neg(value);
	status_reg.apply_zero(value);
}

static void exec(const asr_reg instruction, registers& regs, apsr_reg& status_reg) {
	uint32_t value 			= regs.get(instruction.rdn());
	uint32_t shift_offset 	= bits<0,8>::of(regs.get(instruction.rm())); // max 255

	bool carry = alu::asr_c(value, shift_offset, status_reg.carry_flag());

	regs.set(instruction.rdn(), value);

	status_reg.write_carry_flag(carry);
	status_reg.apply_neg(value);
	status_reg.apply_zero(value);
}

static void exec(const adc instruction, registers& regs, apsr_reg& status_reg) {
	uint32_t rn 	= regs.get(instruction.rdn());
	uint32_t rm 	= regs.get(instruction.rm());

	// left shift of zero is omitted here
	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, rm, status_reg.carry_flag(), carry, overflow);

	regs.set(instruction.rdn(), result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_neg(result);
	status_reg.apply_zero(result);
}

static void exec(const sbc instruction, registers& regs, apsr_reg& status_reg) {
	uint32_t rn 	= regs.get(instruction.rdn());
	uint32_t rm 	= regs.get(instruction.rm());

	// left shift of zero is omitted here
	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, ~rm, status_reg.carry_flag(), carry, overflow);

	regs.set(instruction.rdn(), result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_neg(result);
	status_reg.apply_zero(result);
}

static void exec(const ror_reg instruction, registers& regs, apsr_reg& status_reg) {
	uint32_t value 			= regs.get(instruction.rdn());
	uint32_t shift_offset 	= bits<0,8>::of(regs.get(instruction.rm()));

	bool carry = alu::ror_c(value, shift_offset, status_reg.carry_flag());

	regs.set(instruction.rdn(), value);

	status_reg.write_carry_flag(carry);
	status_reg.apply_neg(value);
	status_reg.apply_zero(value);
}

static void exec(const tst_reg instruction, const registers& regs, apsr_reg& status_reg) {
	uint32_t rn = regs.get(instruction.rn());
	uint32_t rm = regs.get(instruction.rm());

	// left shift of zero is omitted here
	uint32_t result = rn & rm;

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const rsb_imm instruction, registers& regs, apsr_reg& status_reg) {
	uint32_t rn 	= regs.get(instruction.rn());
	uint32_t imm32	= 0; // ARMv6-M only supports a value of 0.

	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(~rn, imm32, true, carry, overflow);

	regs.set(instruction.rd(), result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_neg(result);
	status_reg.apply_zero(result);
}

static void exec(const cmp_reg instruction, const registers& regs, apsr_reg& status_reg) {
	uint32_t rn 	= regs.get(instruction.rn());
	uint32_t rm 	= regs.get(instruction.rm());

	// left shift of zero is omitted here
	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, ~rm, true, carry, overflow);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_neg(result);
	status_reg.apply_zero(result);
}


static void exec(const cmn_reg instruction, const registers& regs, apsr_reg& status_reg) {
	uint32_t rn 	= regs.get(instruction.rn());
	uint32_t rm 	= regs.get(instruction.rm());

	// left shift of zero is omitted here
	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, rm, false, carry, overflow);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_neg(result);
	status_reg.apply_zero(result);
}

static void exec(const orr_reg instruction, registers& regs, apsr_reg& status_reg) {
	uint32_t rd = regs.get(instruction.rdn());
	uint32_t rm = regs.get(instruction.rm());

	// left shift of zero is omitted here
	uint32_t result = rd | rm;

	regs.set(instruction.rdn(), result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const mul_reg instruction, registers& regs, apsr_reg& status_reg) {

	uint32_t rm = regs.get(instruction.rdm());
	uint32_t rn = regs.get(instruction.rn());

	// left shift of zero is omitted here
	uint32_t result = rm * rn;

	regs.set(instruction.rdm(), result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const bic_reg instruction, registers& regs, apsr_reg& status_reg) {

	uint32_t rn = regs.get(instruction.rdn());
	uint32_t rm = regs.get(instruction.rm());

	// left shift of zero is omitted here
	uint32_t result = rn & ~rm;

	regs.set(instruction.rdn(), result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const mvn instruction, registers& regs, apsr_reg& status_reg) {

	uint32_t rm = regs.get(instruction.rm());

	// left shift of zero is omitted here
	uint32_t result = ~rm;

	regs.set(instruction.rd(), result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}

static void exec(const add_highreg instruction, registers& regs) {

	//TODO:
	/* if (DN:Rdn) == '1101' || Rm == '1101' then SEE ADD (SP plus register);
	 *	d = UInt(DN:Rdn); n = d; m = UInt(Rm); setflags = FALSE; (shift_t, shift_n) = (SRType_LSL, 0);
	 *	if n == 15 && m == 15 then UNPREDICTABLE;
	 */


	uint32_t rn = regs.get(instruction.high_rd());
	uint32_t rm = regs.get(instruction.high_rm());
	// left shift of zero is omitted here
	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, rm, false, carry, overflow);


	if(registers::PC == instruction.high_rd()) {
		regs.branch_alu(result);
	} else {
		regs.set(instruction.high_rd(), result);
	}

	// Do not update flags


}

static void exec(const cmp_highreg instruction, const registers& regs, apsr_reg& status_reg) {

	if((instruction.high_rn() < 8) && (instruction.high_rm() < 8)) {
		unpredictable();
	}

	if(registers::PC == instruction.high_rn() || registers::PC == instruction.high_rm()) {
		unpredictable();
	}

	uint32_t rn = regs.get(instruction.high_rn());
	uint32_t rm = regs.get(instruction.high_rm());

	// left shift of zero is omitted here
	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, ~rm, true, carry, overflow);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_neg(result);
	status_reg.apply_zero(result);

}



static void exec(const mov_highreg instruction, registers& regs) {
	/*
	 * Note that if both register ids are smaller than 8, this instruction must be encoded as
	 * a regular mov instruction. It is unpredictable otherwise.
	 */
	reg_idx high_rm = instruction.high_rm();
	uint32_t result = regs.get(high_rm);

	// if rd is PC, just branch
	if(registers::PC == instruction.high_rd()) {

		regs.branch_alu(result);

	} else {

		regs.set(instruction.high_rd(), result);

		// dont update flags
	}
}

static void exec(const bx instruction, registers& regs) {
	if(registers::PC == (reg_idx)instruction.rm()) {
		unpredictable();
	}
	regs.branch_interworking(regs.get(instruction.rm()));
}

static void exec(const blx instruction, registers& regs) {
	if(registers::PC == (reg_idx)instruction.rm()) {
		unpredictable();
	}


	// Compute the return address. PC is two instruction ahead because of prefetch
	uint32_t next_instr_addr = regs.get_pc() - 2;

	// force thumb bit on return address
	bits<0>::of(next_instr_addr) = true;

	// write return address to link register
	regs.set_lr(next_instr_addr);

	// get the address to jump to
	uint32_t jump_addr = regs.get(instruction.rm());

	// jump
	regs.branch_link_interworking(jump_addr);
}

static void exec(const ldr_literal instruction, registers& regs, const memory& mem) {
	uint32_t offset = instruction.imm32();
	uint32_t base 	= binops::aligned(regs.get_pc(), 4);
	uint32_t address = base + offset;
	uint32_t value = mem.read32(address);
	regs.set(instruction.rt(), value);
}

static void exec(const str_reg instruction, const registers& regs, memory& mem) {

	// left shift of zero is omitted here
	uint32_t offset = regs.get(instruction.rm());
	uint32_t base 	= regs.get(instruction.rn());
	uint32_t address = base + offset;
	uint32_t value 	=  regs.get(instruction.rt());
	mem.write32(address, value);
}

static void exec(const strh_reg instruction, const registers& regs, memory& mem) {

	// left shift of zero is omitted here
	uint32_t offset = regs.get(instruction.rm());
	uint32_t base 	= regs.get(instruction.rn());
	uint32_t address = base + offset;
	uint32_t value 	=  regs.get(instruction.rt());
	mem.write16(address, bits<0,16>::of((uint16_t)value));
}

static void exec(const strb_reg instruction, const registers& regs, memory& mem) {

	// left shift of zero is omitted here
	uint32_t offset = regs.get(instruction.rm());
	uint32_t base 	= regs.get(instruction.rn());
	uint32_t address = base + offset;
	uint32_t value 	= regs.get(instruction.rt());
	mem.write8(address, bits<0,8>::of((uint8_t)value));
}

static void exec(const ldrsb_reg instruction, registers& regs, const memory& mem) {
	// left shift of zero is omitted here
	uint32_t offset = regs.get(instruction.rm());
	uint32_t base 	= regs.get(instruction.rn());
	uint32_t address = base + offset;
	bool ok = false;
	int8_t value = mem.read8(address, ok);
	if(ok) {
		regs.set(instruction.rt(), value);
	}
}

static void exec(const ldr_reg instruction, registers& regs, const memory& mem) {
	// left shift of zero is omitted here
	uint32_t offset = regs.get(instruction.rm());
	uint32_t base 	= regs.get(instruction.rn());
	uint32_t address = base + offset;
	bool ok = false;
	uint32_t value = mem.read32(address, ok);
	if(ok) {
		regs.set(instruction.rt(), value);
	}
}

static void exec(const ldrh_reg instruction, registers& regs, const memory& mem) {
	// left shift of zero is omitted here
	uint32_t offset = regs.get(instruction.rm());
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;
	bool ok = false;
	uint16_t value = mem.read16(address, ok);
	if(ok) {
		regs.set(instruction.rt(), (uint32_t) value);
	}
}

static void exec(const ldrb_reg instruction, registers& regs, const memory& mem) {
	// left shift of zero is omitted here
	uint32_t offset = regs.get(instruction.rm());
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;
	bool ok = false;
	uint8_t value = mem.read8(address, ok);
	if(ok) {
		regs.set(instruction.rt(), (uint32_t) value);
	}
}

static void exec(const ldrsh_reg instruction, registers& regs, const memory& mem) {
	// left shift of zero is omitted here
	uint32_t offset = regs.get(instruction.rm());
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;
	bool ok = false;
	int16_t value = mem.read16(address, ok);
	if(ok) {
		regs.set(instruction.rt(), value);
	}
}


static void exec(const str_imm instruction, const registers& regs, memory& mem) {
	uint32_t offset = instruction.imm5() << 2;
	uint32_t base 	= regs.get(instruction.rn());
	uint32_t address = base + offset;
	uint32_t value 	=  regs.get(instruction.rt());
	mem.write32(address, value);
}

static void exec(const ldr_imm instruction, registers& regs, const memory& mem) {
	uint32_t offset = instruction.imm32();
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;
	bool ok = false;
	uint32_t value = mem.read32(address, ok);
	if (ok) {
		regs.set(instruction.rt(), value);
	}
}

static void exec(const strb_imm instruction, const registers& regs, memory& mem) {
	uint32_t offset = instruction.imm5();
	uint32_t base 	= regs.get(instruction.rn());
	uint32_t address = base + offset;
	uint32_t value 	=  regs.get(instruction.rt());
	mem.write8(address, bits<0,8>::of((uint8_t)value));
}

static void exec(const ldrb_imm instruction, registers& regs, const memory& mem) {
	uint32_t offset = instruction.imm5();
	uint32_t base 	= regs.get(instruction.rn());
	uint32_t address = base + offset;
	bool ok = false;
	uint32_t value = mem.read8(address, ok);

	if(ok) {
		regs.set(instruction.rt(), value);
	}
}

static void exec(const strh_imm instruction, const registers& regs, memory& mem) {
	uint32_t offset = instruction.imm5() << 1;
	uint32_t base 	= regs.get(instruction.rn());
	uint32_t address = base + offset;
	uint32_t value 	=  regs.get(instruction.rt());
	mem.write16(address, bits<0,16>::of((uint16_t)value));
}

static void exec(const ldrh_imm instruction, registers& regs, const memory& mem) {
	uint32_t offset = instruction.imm5() << 1;
	uint32_t base 	= regs.get(instruction.rn());
	uint32_t address = base + offset;
	bool ok = false;
	uint32_t value = mem.read16(address, ok);
	if(ok) {
		regs.set(instruction.rt(), value);
	}
}

static void exec(const str_sp_imm instruction, const registers& regs, memory& mem) {
	uint32_t offset = instruction.imm32();
	uint32_t base 	= regs.get_sp(); // SP
	uint32_t address = base + offset;
	uint32_t value 	=  regs.get(instruction.rt());
	mem.write32(address, value);
}

static void exec(const ldr_sp_imm instruction, registers& regs, const memory& mem) {
	uint32_t offset = instruction.imm8() << 2;
	uint32_t base 	= regs.get_sp(); // SP
	uint32_t address = base + offset;
	uint32_t value = mem.read32(address);
	regs.set(instruction.rt(), value);
}

static void exec(const adr instruction, registers& regs) {
	uint32_t offset = instruction.imm32();
	uint32_t base 	= binops::aligned(regs.get_pc(), 4); // PC
	uint32_t address = base + offset;
	regs.set(instruction.rd(), address);
}


static void exec(const add_sp_imm instruction, registers& regs) {
	// TODO check if other instructions should go here
	uint32_t imm32 = instruction.imm32();
	uint32_t sp = regs.get_sp();
	uint32_t result = alu::add_with_carry(sp, imm32, false);
	regs.set(instruction.rd(), result);
}

static void exec(const add_sp_imm_t2 instruction, registers& regs) {
	// TODO check if other instructions should go here
	uint32_t imm32 = instruction.imm32();
	uint32_t sp = regs.get_sp();
	uint32_t result = alu::add_with_carry(sp, imm32, false);
	regs.set_sp(result);
}

static void exec(const sub_sp_imm instruction, registers& regs) {
	// TODO check if other instructions should go here
	uint32_t imm32 = instruction.imm32();
	uint32_t sp = regs.get_sp();
	uint32_t result = alu::add_with_carry(sp, ~imm32, true);
	regs.set_sp(result);
}

static void exec(const sxth instruction, registers& regs) {
	// Note: ROR(0) is omitted here
	uint32_t rm = regs.get(instruction.rm());
	uint32_t extended = binops::sign(rm, 16); // sign-extend 16 bit to 32
	regs.set(instruction.rd(), extended);
}

static void exec(const sxtb instruction, registers& regs) {
	// Note: ROR(0) is omitted here
	uint32_t rm = regs.get(instruction.rm());
	uint32_t extended = binops::sign(rm, 8); // sign-extend 8 bit to 32
	regs.set(instruction.rd(), extended);
}

static void exec(const uxth instruction, registers& regs) {
	// Note: ROR(0) is omitted here
	uint16_t rm = bits<0,16>::of(regs.get(instruction.rm()));
	uint32_t extended(rm); // zero-extend 16 bit to 32
	regs.set(instruction.rd(), extended);
}

static void exec(const uxtb instruction, registers& regs) {
	// Note: ROR(0) is omitted here
	uint8_t rm = bits<0,8>::of(regs.get(instruction.rm()));
	uint32_t extended(rm); // zero-extend 8 bit to 32
	regs.set(instruction.rd(), extended);
}

static void exec(const push instruction, registers& regs, memory& mem) {
	precond(instruction.pop_count() > 0, "must push at least one register");
	const size_t stored_size = 4 * instruction.pop_count();
	const uint32_t start_address = regs.get_sp() - stored_size;
	//fprintf(stderr, "push stack %08X - %08X\n", start_address, regs.get_sp());
	size_t count = 0;
	for(reg_idx rid = 0; rid < registers::NUM_REGS-1; rid++) {
		if(instruction.is_set(rid)) {
			uint32_t val = regs.get(rid);
			mem.write32(start_address + (count*4), val);
			count++;
		}
	}
	regs.set_sp(start_address);
}

static void exec(const cps instruction, registers& regs) {
	bits<0>::of(regs.primask_register()) = instruction.im;
}

static void exec(const pop instruction, registers& regs, memory& mem) {
	precond(instruction.pop_count() > 0, "must push at least one register");
	const uint32_t frame_start = regs.get_sp(); // sp
	uint32_t base = frame_start;
	const uint32_t stored_size = 4 * instruction.pop_count();
	//fprintf(stderr, "pop stack %08X - %08X\n", frame_start, frame_start + stored_size);
	for (reg_idx rid = 0; rid < 8; rid++) {
		if(instruction.is_set(rid)) {
			regs.set(rid, mem.read32(base));
			base += 4;
		}
	}

	// TODO: Check if this is indeed the correct behavior
	// WARNING: The spec says that SP should be
	// udpdated AFTER loading of SP (the code below)
	// This is problematic upon exception return
	// because the context will be restored by poping
	// a 32bytes conext from the stack, relative to SP.
	// If SP is not updated BEFORE, poping the context
	// will start from an incorrect address.
	// To fix this, the SP update has been moved BEFORE
	// loading the SP.
	regs.set_sp(frame_start + stored_size);

	// PC is always the last, so base value
	// is what needs to be written.
	if(instruction.is_set(registers::PC)) {

		/* TODO: check whats going on with
		 * thumb bit when popping PC from
		 * a previous push LR where LR = 0
		 */
		uint32_t address = mem.read32(base);
		if(address == 0xFFFFFFF9) {
			int k = 0;
		}
		//fprintf(stderr, "pop pc: %08x\n", address);
		//address.set_bit(0); // fix thumb bit ??'
		regs.branch_interworking(address);
	}


}

static void exec(const bkpt instruction, bool& break_signal) {
	break_signal = true;
	fprintf(stderr, "BREAKPOINT %d\n", instruction.imm8().extract());
}

static void exec(const rev_word instruction, registers& regs) {
	regs.set(instruction.rd(), binops::swap(regs.get(instruction.rm())));
}

static void exec(const rev16 instruction, registers& regs) {

	const uint32_t rm = regs.get(instruction.rm());

	uint32_t res =  bits<16,8>::of((rm)) << 24 |
				bits<24,8>::of((rm)) << 16 |
				bits<0,8>::of((rm)) << 8   |
				bits<8,8>::of((rm)) << 0;

	regs.set(instruction.rd(), res);
}

static void exec(const revsh instruction, registers& regs) {

	const uint32_t rm = regs.get(instruction.rm());

	uint32_t swapped_low16 = (bits<0,8>::of(rm) << 8) | bits<8,8>::of(rm);
	uint32_t res = binops::sign(swapped_low16, 16);

	regs.set(instruction.rd(), res);
}

static void exec(const branch instruction, registers& regs, apsr_reg& flags) {


	if(!flags.condition_passed(instruction.cond())) {
		return;
	}

	int32_t delta = instruction.offset();
	uint32_t pc = regs.get_pc();
	regs.set_pc(pc + delta);
}

static void exec(const unconditional_branch instruction, registers& regs) {
	int32_t delta = instruction.offset();
	uint32_t pc = regs.get_pc();
	regs.set_pc(pc + delta);
}

static void exec(const stm instruction, registers& regs, memory& mem) {
	uint32_t address = regs.get(instruction.rn());

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
	regs.set(instruction.rn(), address);
}


static void exec(const ldm instruction, registers& regs, memory& mem) {
	uint32_t address = regs.get(instruction.rn());

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
	if(!instruction.is_set(instruction.rn())) {
		regs.set(instruction.rn(), address);
	}
}


static void exec(const mrs instruction, registers& regs) {
	// do not keep the value initially present in the register
	uint32_t val = 0;
	uint8_t instr_sysn = instruction.sysn;
	uint8_t sysn_bits = bits<3,5>::of(instruction.sysn);
	switch(sysn_bits) {
		case 0b00000: {
			if(bits<0>::of(instruction.sysn)) {
				bits<0,8>::of(val) = bits<0,8>::of((uint32_t)regs.interrupt_status_register().exception_num());
			}
			if(bits<1>::of(instruction.sysn)) {
				// T-bit reads as zero
				bits<epsr_reg::THUMB_BIT>::of(val) = false;
			}
			if(!bits<2>::of(instruction.sysn)) {
				// T-bit reads as zero
				// copy 5 bits from APS
				// TODO: Why 5 and not 4 ???
				bits<27, 5>::of(val) = bits<27, 5>::of(regs.xpsr_register());
			}
		} break;
		case 0b00001: {
			switch((uint8_t)bits<0,3>::of(instruction.sysn)) {
				case 0b000: {
					val = regs.sp_register().get_specific_banked_sp(sp_reg::StackType::Main);
				} break;
				case 0b001: {
					val = regs.sp_register().get_specific_banked_sp(sp_reg::StackType::Process);
					fprintf(stderr, "%08x\n", val);
				} break;
			}
		} break;
		case 0b00010: {
			switch((uint8_t)bits<0,3>::of(instruction.sysn)) {
				case 0b000: {
					val = bits<0>::of(regs.primask_register());
				} break;
				case 0b001: {
					bits<0, 2>::of(val) = bits<0, 2>::of((uint32_t)regs.control_register());
				} break;
			}
		} break;
		default: {
			// unpredictable
			break;
		}
	}
	regs.set(instruction.rd, val);
}

static void exec(const msr instruction, registers& regs, apsr_reg& apsr) {
	switch(instruction.sysn) {
		case msr::SpecialRegister::APSR:
		case msr::SpecialRegister::IAPSR:
		case msr::SpecialRegister::EAPSR:
		case msr::SpecialRegister::XPSR: {
			apsr.copy_bits(bits<0,5>::of(regs.get(instruction.rn)));
		} break;
		case msr::SpecialRegister::MSP: {
			// TODO: Should fail if not in privileged mode
			uint32_t sp = bits<2,30>::of(regs.get(instruction.rn)) << 2;
			regs.sp_register().set_specific_banked_sp(sp_reg::StackType::Main, sp);
		} break;
		case msr::SpecialRegister::PSP: {
			// TODO: Should fail if not in privileged mode
			uint32_t sp = bits<2,30>::of(regs.get(instruction.rn));
			regs.sp_register().set_specific_banked_sp(sp_reg::StackType::Process, sp);
		} break;
		case msr::SpecialRegister::PRIMASK: {
			// TODO: MSR SpecialRegister::PRIMASK
			bits<0>::of(regs.primask_register()) = bits<0>::of(regs.get(instruction.rn));
		} break;
		case msr::SpecialRegister::CONTROL: {
			if(regs.exec_mode_register().is_thread_mode()) {
				uint32_t val = regs.get(instruction.rn);
				regs.control_register().set_n_priv(bits<0>::of(val));
				regs.control_register().set_sp_sel(bits<1>::of(val));
			}
		} break;
		default: {
			// unpredictable
			break;
		}
	}
}

static void exec(const bl_imm instruction, registers& regs) {
	// pc is 4 bytes ahead, so already poiting to the next instruction
	uint32_t next_instr_addr = regs.get_pc();
	bits<0>::of(next_instr_addr) = true; // force thumb mode
	regs.set_lr(next_instr_addr);
	int32_t offset = instruction.offset();
	int32_t new_pc = regs.get_pc() + offset;
	regs.set_pc(new_pc);
}

#endif //THUMBEMU_EXEC_HPP

