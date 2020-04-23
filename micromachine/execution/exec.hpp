//
// Created by fla on 17-5-4.
//

#pragma once

#include "alu.hpp"
#include "helpers/binops.hpp"
#include "instruction/instructions.hpp"
#include "interworking_brancher.hpp"
#include "memory/memory.hpp"
#include "registers/core_registers.hpp"
#include "registers/event_register.hpp"
#include "registers/special_registers.hpp"
#include "types/bits.hpp"
#include "types/types.hpp"

namespace micromachine::system {

static void unpredictable() {
	// TODO:
}

static void exec(const lsl_imm instruction, core_registers& regs, apsr_reg& status_reg) {
	uint32_t rm = regs.get(instruction.rm());
	const bool carry = alu::lsl_c(rm, instruction.shift_offset(), status_reg.carry_flag());
	status_reg.apply_neg(rm);
	status_reg.apply_zero(rm);
	status_reg.write_carry_flag(carry);
	regs.set(instruction.rd(), rm);
}
static void exec(const lsr_imm instruction, core_registers& regs, apsr_reg& status_reg) {
	uint32_t rm = regs.get(instruction.rm());
	uint32_t applied_offset = instruction.shift_offset();
	if(applied_offset == 0) {
		applied_offset = 32;
	}
	const bool carry = alu::lsr_c(rm, applied_offset, status_reg.carry_flag());
	status_reg.apply_neg(rm);
	status_reg.apply_zero(rm);
	status_reg.write_carry_flag(carry);
	regs.set(instruction.rd(), rm);
}
static void exec(const asr_imm instruction, core_registers& regs, apsr_reg& status_reg) {
	uint32_t rm = regs.get(instruction.rm());
	uint32_t applied_offset = instruction.shift_offset();
	if(applied_offset == 0) {
		applied_offset = 32;
	}
	const bool carry = alu::asr_c(rm, applied_offset, status_reg.carry_flag());
	status_reg.apply_neg(rm);
	status_reg.apply_zero(rm);
	status_reg.write_carry_flag(carry);
	regs.set(instruction.rd(), rm);
}
static void exec(const add_reg instruction, core_registers& regs, apsr_reg& status_reg) {
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
static void exec(const subs_reg instruction, core_registers& regs, apsr_reg& status_reg) {
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
static void exec(const add_imm instruction, core_registers& regs, apsr_reg& status_reg) {

	uint32_t rn = regs.get(instruction.rn());
	uint32_t imm32 = instruction.imm3();

	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, imm32, false, carry, overflow);

	regs.set(instruction.rd(), result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}
static void exec(const subs_imm instruction, core_registers& regs, apsr_reg& status_reg) {
	uint32_t rn = regs.get(instruction.rn());
	uint32_t imm32 = instruction.imm3();

	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, ~imm32, true, carry, overflow);

	regs.set(instruction.rd(), result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}
static void exec(const mov_imm instruction, core_registers& regs, apsr_reg& status_reg) {
	uint32_t imm32 = instruction.imm8();
	regs.set(instruction.rd(), instruction.imm8());
	status_reg.apply_zero(imm32);
	status_reg.apply_neg(imm32);
}
static void exec(const movs instruction, core_registers& regs, apsr_reg& status_reg) {
	uint32_t val = regs.get(instruction.rm());
	regs.set(instruction.rd(), val);
	status_reg.apply_zero(val);
	status_reg.apply_neg(val);
}
static void exec(const cmp_imm instruction, const core_registers& regs, apsr_reg& status_reg) {
	// like subtraction, but discard the result
	uint32_t imm32 = instruction.imm8();
	uint32_t rn = regs.get(instruction.rn());
	bool carry = false;
	bool overflow = false;

	uint32_t result = alu::add_with_carry(rn, ~imm32, true, carry, overflow);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}
static void exec(const add_imm_t2 instruction, core_registers& regs, apsr_reg& status_reg) {

	uint32_t rn = regs.get(instruction.rdn());
	uint32_t imm32 = instruction.imm8();

	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, imm32, false, carry, overflow);

	regs.set(instruction.rdn(), result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}
static void exec(const subs_imm8 instruction, core_registers& regs, apsr_reg& status_reg) {

	uint32_t rn = regs.get(instruction.rdn());
	uint32_t imm32 = instruction.imm8();

	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, ~imm32, true, carry, overflow);

	regs.set(instruction.rdn(), result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}
static void exec(const and_reg instruction, core_registers& regs, apsr_reg& status_reg) {

	uint32_t rd = regs.get(instruction.rdn());
	uint32_t rm = regs.get(instruction.rm());

	// left shift of zero is omitted here
	uint32_t result = rd & rm;

	regs.set(instruction.rdn(), result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}
static void exec(const eor_reg instruction, core_registers& regs, apsr_reg& status_reg) {

	uint32_t rd = regs.get(instruction.rdn());
	uint32_t rm = regs.get(instruction.rm());

	// left shift of zero is omitted here
	uint32_t result = rd ^ rm;

	regs.set(instruction.rdn(), result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}
static void exec(const lsl_reg instruction, core_registers& regs, apsr_reg& status_reg) {
	uint32_t value = regs.get(instruction.rdn());
	uint32_t shift_offset = bits<0, 8>::of(regs.get(instruction.rm()));

	bool carry = alu::lsl_c(value, shift_offset, status_reg.carry_flag());

	regs.set(instruction.rdn(), value);

	status_reg.write_carry_flag(carry);
	status_reg.apply_neg(value);
	status_reg.apply_zero(value);
}
static void exec(const lsr_reg instruction, core_registers& regs, apsr_reg& status_reg) {
	uint32_t value = regs.get(instruction.rdn());
	uint32_t shift_offset = bits<0, 8>::of(regs.get(instruction.rm()));

	bool carry = alu::lsr_c(value, shift_offset, status_reg.carry_flag());

	regs.set(instruction.rdn(), value);

	status_reg.write_carry_flag(carry);
	status_reg.apply_neg(value);
	status_reg.apply_zero(value);
}
static void exec(const asr_reg instruction, core_registers& regs, apsr_reg& status_reg) {
	uint32_t value = regs.get(instruction.rdn());
	uint32_t shift_offset = bits<0, 8>::of(regs.get(instruction.rm())); // max 255

	bool carry = alu::asr_c(value, shift_offset, status_reg.carry_flag());

	regs.set(instruction.rdn(), value);

	status_reg.write_carry_flag(carry);
	status_reg.apply_neg(value);
	status_reg.apply_zero(value);
}
static void exec(const adc instruction, core_registers& regs, apsr_reg& status_reg) {
	uint32_t rn = regs.get(instruction.rdn());
	uint32_t rm = regs.get(instruction.rm());

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
static void exec(const sbc instruction, core_registers& regs, apsr_reg& status_reg) {
	uint32_t rn = regs.get(instruction.rdn());
	uint32_t rm = regs.get(instruction.rm());

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
static void exec(const ror_reg instruction, core_registers& regs, apsr_reg& status_reg) {
	uint32_t value = regs.get(instruction.rdn());
	uint32_t shift_offset = bits<0, 8>::of(regs.get(instruction.rm()));

	bool carry = alu::ror_c(value, shift_offset, status_reg.carry_flag());

	regs.set(instruction.rdn(), value);

	status_reg.write_carry_flag(carry);
	status_reg.apply_neg(value);
	status_reg.apply_zero(value);
}
static void exec(const tst_reg instruction, const core_registers& regs, apsr_reg& status_reg) {
	uint32_t rn = regs.get(instruction.rn());
	uint32_t rm = regs.get(instruction.rm());

	// left shift of zero is omitted here
	uint32_t result = rn & rm;

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}
static void exec(const rsb_imm instruction, core_registers& regs, apsr_reg& status_reg) {
	uint32_t rn = regs.get(instruction.rn());
	uint32_t imm32 = 0; // ARMv6-M only supports a value of 0.

	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(~rn, imm32, true, carry, overflow);

	regs.set(instruction.rd(), result);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_neg(result);
	status_reg.apply_zero(result);
}
static void exec(const cmp_reg instruction, const core_registers& regs, apsr_reg& status_reg) {
	uint32_t rn = regs.get(instruction.rn());
	uint32_t rm = regs.get(instruction.rm());

	// left shift of zero is omitted here
	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, ~rm, true, carry, overflow);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_neg(result);
	status_reg.apply_zero(result);
}
static void exec(const cmn_reg instruction, const core_registers& regs, apsr_reg& status_reg) {
	uint32_t rn = regs.get(instruction.rn());
	uint32_t rm = regs.get(instruction.rm());

	// left shift of zero is omitted here
	bool carry = false;
	bool overflow = false;
	uint32_t result = alu::add_with_carry(rn, rm, false, carry, overflow);

	status_reg.write_carry_flag(carry);
	status_reg.write_overflow_flag(overflow);
	status_reg.apply_neg(result);
	status_reg.apply_zero(result);
}
static void exec(const orr_reg instruction, core_registers& regs, apsr_reg& status_reg) {
	uint32_t rd = regs.get(instruction.rdn());
	uint32_t rm = regs.get(instruction.rm());

	// left shift of zero is omitted here
	uint32_t result = rd | rm;

	regs.set(instruction.rdn(), result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}
static void exec(const mul_reg instruction, core_registers& regs, apsr_reg& status_reg) {

	uint32_t rm = regs.get(instruction.rdm());
	uint32_t rn = regs.get(instruction.rn());

	// left shift of zero is omitted here
	uint32_t result = rm * rn;

	regs.set(instruction.rdm(), result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}
static void exec(const bic_reg instruction, core_registers& regs, apsr_reg& status_reg) {

	uint32_t rn = regs.get(instruction.rdn());
	uint32_t rm = regs.get(instruction.rm());

	// left shift of zero is omitted here
	uint32_t result = rn & ~rm;

	regs.set(instruction.rdn(), result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}
static void exec(const mvn instruction, core_registers& regs, apsr_reg& status_reg) {

	uint32_t rm = regs.get(instruction.rm());

	// left shift of zero is omitted here
	uint32_t result = ~rm;

	regs.set(instruction.rd(), result);

	status_reg.apply_zero(result);
	status_reg.apply_neg(result);
}
static void exec(const add_highreg instruction, core_registers& regs) {

	// TODO:
	/* if (DN:Rdn) == '1101' || Rm == '1101' then SEE ADD (SP plus register);
	 *	d = UInt(DN:Rdn); n = d; m = UInt(Rm); setflags = FALSE; (shift_t, shift_n) = (SRType_LSL,
	 *0); if n == 15 && m == 15 then UNPREDICTABLE;
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
static void exec(const cmp_highreg instruction, const core_registers& regs, apsr_reg& status_reg) {

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
static void exec(const mov_highreg instruction, core_registers& regs) {
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
static void
exec(const bx instruction, core_registers& regs, interworking_brancher& interworking_brancher) {
	if(registers::PC == (reg_idx)instruction.rm()) {
		unpredictable();
	}
	interworking_brancher.branch_interworking(regs.get(instruction.rm()));
}
static void
exec(const blx instruction, core_registers& regs, interworking_brancher& interworking_brancher) {
	if(registers::PC == (reg_idx)instruction.rm()) {
		unpredictable();
	}

	// Compute the return address. PC is two instruction ahead because of prefetch
	uint32_t next_instr_addr = regs.get_pc() - 2; // 16386

	// force thumb bit on return address
	bits<0>::of(next_instr_addr) = true;

	// get the address to jump to
	uint32_t jump_addr = regs.get(instruction.rm()); // jump_addr 16401

	// write return address to link register
	regs.set_lr(next_instr_addr);

	// jump
	interworking_brancher.branch_link_interworking(jump_addr);
}
static void exec(const ldr_literal instruction, core_registers& regs, const memory& mem) {
	uint32_t offset = instruction.imm32();
	uint32_t base = binops::aligned<4>(regs.get_pc());
	uint32_t address = base + offset;
	uint32_t value = mem.read32(address);
	regs.set(instruction.rt(), value);
}
static void exec(const str_reg instruction, const core_registers& regs, memory& mem) {

	// left shift of zero is omitted here
	uint32_t offset = regs.get(instruction.rm());
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;
	uint32_t value = regs.get(instruction.rt());
	mem.write32(address, value);
}
static void exec(const strh_reg instruction, const core_registers& regs, memory& mem) {

	// left shift of zero is omitted here
	uint32_t offset = regs.get(instruction.rm());
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;
	uint32_t value = regs.get(instruction.rt());
	mem.write16(address, bits<0, 16>::of((uint16_t)value));
}
static void exec(const strb_reg instruction, const core_registers& regs, memory& mem) {

	// left shift of zero is omitted here
	uint32_t offset = regs.get(instruction.rm());
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;
	uint32_t value = regs.get(instruction.rt());
	mem.write8(address, bits<0, 8>::of((uint8_t)value));
}
static void exec(const ldrsb_reg instruction, core_registers& regs, const memory& mem) {
	// left shift of zero is omitted here
	uint32_t offset = regs.get(instruction.rm());
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;
	bool memory_access_success = false;
	int8_t value = mem.read8(address, memory_access_success);
	if(memory_access_success) {
		regs.set(instruction.rt(), value);
	}
}
static void exec(const ldr_reg instruction, core_registers& regs, const memory& mem) {
	// left shift of zero is omitted here
	uint32_t offset = regs.get(instruction.rm());
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;
	bool memory_access_success = false;
	uint32_t value = mem.read32(address, memory_access_success);
	if(memory_access_success) {
		regs.set(instruction.rt(), value);
	}
}
static void exec(const ldrh_reg instruction, core_registers& regs, const memory& mem) {
	// left shift of zero is omitted here
	uint32_t offset = regs.get(instruction.rm());
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;
	bool memory_access_success = false;
	uint16_t value = mem.read16(address, memory_access_success);
	if(memory_access_success) {
		regs.set(instruction.rt(), (uint32_t)value);
	}
}
static void exec(const ldrb_reg instruction, core_registers& regs, const memory& mem) {
	// left shift of zero is omitted here
	uint32_t offset = regs.get(instruction.rm());
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;
	bool memory_access_success = false;
	uint8_t value = mem.read8(address, memory_access_success);
	if(memory_access_success) {
		regs.set(instruction.rt(), (uint32_t)value);
	}
}
static void exec(const ldrsh_reg instruction, core_registers& regs, const memory& mem) {
	// left shift of zero is omitted here
	uint32_t offset = regs.get(instruction.rm());
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;
	bool memory_access_success = false;
	int16_t value = mem.read16(address, memory_access_success);
	if(memory_access_success) {
		regs.set(instruction.rt(), value);
	}
}
static void exec(const str_imm instruction, const core_registers& regs, memory& mem) {
	uint32_t offset = instruction.imm5() << 2U;
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;
	uint32_t value = regs.get(instruction.rt());
	mem.write32(address, value);
}
static void exec(const ldr_imm instruction, core_registers& regs, const memory& mem) {
	uint32_t offset = instruction.imm32();
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;

	bool memory_access_success = false;
	uint32_t value = mem.read32(address, memory_access_success);
	if(memory_access_success) {
		regs.set(instruction.rt(), value);
	}
}
static void exec(const strb_imm instruction, const core_registers& regs, memory& mem) {
	uint32_t offset = instruction.imm5();
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;
	uint32_t value = regs.get(instruction.rt());
	mem.write8(address, bits<0, 8>::of((uint8_t)value));
}
static void exec(const ldrb_imm instruction, core_registers& regs, const memory& mem) {
	uint32_t offset = instruction.imm5();
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;

	bool memory_access_success = false;
	uint32_t value = mem.read8(address, memory_access_success);
	if(memory_access_success) {
		regs.set(instruction.rt(), value);
	}
}
static void exec(const strh_imm instruction, const core_registers& regs, memory& mem) {
	uint32_t offset = instruction.imm5() << 1U;
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;
	uint32_t value = regs.get(instruction.rt());
	mem.write16(address, bits<0, 16>::of((uint16_t)value));
}
static void exec(const ldrh_imm instruction, core_registers& regs, const memory& mem) {
	uint32_t offset = instruction.imm5() << 1U;
	uint32_t base = regs.get(instruction.rn());
	uint32_t address = base + offset;

	bool memory_access_success = false;
	uint32_t value = mem.read16(address, memory_access_success);
	if(memory_access_success) {
		regs.set(instruction.rt(), value);
	}
}
static void exec(const str_sp_imm instruction, const core_registers& regs, memory& mem) {
	uint32_t offset = instruction.imm32();
	uint32_t base = regs.get_sp(); // SP
	uint32_t address = base + offset;
	uint32_t value = regs.get(instruction.rt());
	mem.write32(address, value);
}
static void exec(const ldr_sp_imm instruction, core_registers& regs, const memory& mem) {
	uint32_t offset = instruction.imm8() << 2U;
	uint32_t base = regs.get_sp(); // SP
	uint32_t address = base + offset;
	uint32_t value = mem.read32(address);
	regs.set(instruction.rt(), value);
}
static void exec(const adr instruction, core_registers& regs) {
	uint32_t offset = instruction.imm32();
	uint32_t base = binops::aligned<4>(regs.get_pc()); // PC
	uint32_t address = base + offset;
	regs.set(instruction.rd(), address);
}
static void exec(const add_sp_imm instruction, core_registers& regs) {
	// TODO check if other instructions should go here
	uint32_t imm32 = instruction.imm32();
	uint32_t sp = regs.get_sp();
	uint32_t result = alu::add_with_carry(sp, imm32, false);
	regs.set(instruction.rd(), result);
}
static void exec(const add_sp_imm_t2 instruction, core_registers& regs) {
	// TODO check if other instructions should go here
	uint32_t imm32 = instruction.imm32();
	uint32_t sp = regs.get_sp();
	uint32_t result = alu::add_with_carry(sp, imm32, false);
	regs.set_sp(result);
}
static void exec(const sub_sp_imm instruction, core_registers& regs) {
	// TODO check if other instructions should go here
	uint32_t imm32 = instruction.imm32();
	uint32_t sp = regs.get_sp();
	uint32_t result = alu::add_with_carry(sp, ~imm32, true);
	regs.set_sp(result);
}
static void exec(const sxth instruction, core_registers& regs) {
	// Note: ROR(0) is omitted here
	uint32_t rm = regs.get(instruction.rm());
	uint32_t extended = binops::sign(rm, 16); // sign-extend 16 bit to 32
	regs.set(instruction.rd(), extended);
}
static void exec(const sxtb instruction, core_registers& regs) {
	// Note: ROR(0) is omitted here
	uint32_t rm = regs.get(instruction.rm());
	uint32_t extended = binops::sign(rm, 8); // sign-extend 8 bit to 32
	regs.set(instruction.rd(), extended);
}
static void exec(const uxth instruction, core_registers& regs) {
	// Note: ROR(0) is omitted here
	uint16_t rm = bits<0, 16>::of(regs.get(instruction.rm()));
	uint32_t extended(rm); // zero-extend 16 bit to 32
	regs.set(instruction.rd(), extended);
}
static void exec(const uxtb instruction, core_registers& regs) {
	// Note: ROR(0) is omitted here
	uint8_t rm = bits<0, 8>::of(regs.get(instruction.rm()));
	uint32_t extended(rm); // zero-extend 8 bit to 32
	regs.set(instruction.rd(), extended);
}
static void exec(const push instruction, core_registers& regs, memory& mem) {
	micromachine_check(instruction.pushed_registers_count() > 0, "must push at least one register");
	const size_t stored_size = 4 * instruction.pushed_registers_count();
	const uint32_t start_address = regs.get_sp() - stored_size;

	size_t count = 0;
	// TODO This loop can be optimized to check only the appropriate bits 0,1,2,3,4,5,6,7,14 of the
	// bitfield
	for(reg_idx rid = 0; rid < core_registers::NUM_CORE_REGS - 1; rid++) {
		if(instruction.is_set(rid)) {
			uint32_t val = regs.get(rid);
			mem.write32(start_address + (count * 4), val);
			count++;
		}
	}
	regs.set_sp(start_address);
}
static void exec(const cps instruction, special_registers& special_regs) {
	special_regs.primask_register().set_pm(instruction.im);
}
static void exec(const pop instruction,
				 core_registers& regs,
				 memory& mem,
				 interworking_brancher& _interworking_brancher) {
	micromachine_check(instruction.pop_count() > 0, "must push at least one register");
	const uint32_t frame_start = regs.get_sp(); // sp
	uint32_t sp_base = frame_start;
	const uint32_t stored_size = 4 * instruction.pop_count();
	// fprintf(stderr, "pop stack %08X - %08X\n", frame_start, frame_start + stored_size);
	for(reg_idx rid = 0; rid < 8; rid++) {
		if(instruction.is_set(rid)) {
			regs.set(rid, mem.read32(sp_base));
			sp_base += 4;
		}
	}

	// TODO: Check if this is indeed the correct behavior
	// WARNING: The spec says that the stack should be incremented
	// updated AFTER loading SP with the value present on the stack
	// This is problematic upon exception return
	// because the context will be restored by poping
	// a 32bytes context from the stack, relative to SP.
	// If SP is not updated BEFORE, poping the context
	// will start from an incorrect address.
	// To fix this, the SP update has been moved BEFORE
	// loading the SP.

	// increment sp by stored_size (=pop the stack)
	regs.set_sp(frame_start + stored_size);

	if(instruction.is_set(registers::PC)) {

		/* TODO: check whats going on with
		 * thumb bit when popping PC from
		 * a previous push LR where LR = 0
		 */

		// here sp base points to saved value of PC
		uint32_t address = mem.read32(sp_base);
		if(address == 0xFFFFFFF9) {
			int k = 0;
		}
		// fprintf(stderr, "pop pc: %08x\n", address);
		// address.set_bit(0); // fix thumb bit ??'
		_interworking_brancher.branch_interworking(address);
	}
}
static void exec(const bkpt instruction, bool& break_signal) {
	break_signal = true;
	// fprintf(stderr, "BREAKPOINT %d\n", instruction.imm8().extract());
}
static void exec(const rev_word instruction, core_registers& regs) {
	regs.set(instruction.rd(), binops::swap(regs.get(instruction.rm())));
}
static void exec(const rev16 instruction, core_registers& regs) {

	const uint32_t rm = regs.get(instruction.rm());

	uint32_t res = bits<16, 8>::of((rm)) << 24U | bits<24, 8>::of((rm)) << 16U |
				   bits<0, 8>::of((rm)) << 8U | bits<8, 8>::of((rm)) << 0U;

	regs.set(instruction.rd(), res);
}
static void exec(const revsh instruction, core_registers& regs) {

	const uint32_t rm = regs.get(instruction.rm());

	uint32_t swapped_low16 = (bits<0, 8>::of(rm) << 8U) | bits<8, 8>::of(rm);
	uint32_t res = binops::sign(swapped_low16, 16);

	regs.set(instruction.rd(), res);
}
static void exec(const branch instruction, core_registers& regs, apsr_reg& flags) {

	if(!flags.condition_passed(instruction.cond())) {
		return;
	}

	int32_t delta = instruction.offset();
	uint32_t pc = regs.get_pc();
	regs.set_pc(pc + delta);
}
static void exec(const unconditional_branch instruction, core_registers& regs) {
	int32_t delta = instruction.offset();
	uint32_t pc = regs.get_pc();
	regs.set_pc(pc + delta);
}
static void exec(const stm instruction, core_registers& regs, memory& mem) {
	uint32_t address = regs.get(instruction.rn());

	for(reg_idx rid = 0; rid < core_registers::NUM_LOW_GP_REGS; rid++) {
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
static void exec(const ldm instruction, core_registers& regs, memory& mem) {
	uint32_t address = regs.get(instruction.rn());

	for(reg_idx rid = 0; rid < core_registers::NUM_LOW_GP_REGS; rid++) {
		if(instruction.is_set(rid)) {
			// dont modify register if memory access is faulty
			bool memory_access_success = false;
			uint32_t value = mem.read32(address, memory_access_success);
			if(memory_access_success) {
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
static void
exec(const mrs instruction, core_registers& core_regs, special_registers& special_regs) {
	// do not keep the value initially present in the register
	uint32_t val = 0;
	uint8_t sysn_bits = bits<3, 5>::of(instruction.sysn);
	switch(sysn_bits) {
		case 0b00000: {                         // xPSR composites
			if(bits<0>::of(instruction.sysn)) { // add IPSR bits
				bits<0, 8>::of(val) = bits<0, 8>::of(
					(uint32_t)special_regs.interrupt_status_register().exception_num());
			}
			if(bits<1>::of(instruction.sysn)) { // add EPSR bits
				// T-bit reads as zero
				bits<epsr_reg::THUMB_BIT>::of(val) = false;
			}
			if(!bits<2>::of(instruction.sysn)) { // add APSR bits
				// T-bit reads as zero
				// copy 5 bits from APS
				// TODO: Why 5 and not 4 ???
				bits<27, 5>::of(val) = bits<27, 5>::of(special_regs.xpsr_register());
			}
		} break;
		case 0b00001: { // SP (exclusive)
			switch((uint8_t)bits<0, 3>::of(instruction.sysn)) {
				case 0b000: { // add MSP bits
					val = core_regs.sp_register().get_specific_banked_sp(sp_reg::StackType::Main);
				} break;
				case 0b001: { // add PSP bits
					val =
						core_regs.sp_register().get_specific_banked_sp(sp_reg::StackType::Process);
					fprintf(stderr, "%08x\n", val);
				} break;
				default: {
					// unpredictable
					break;
				}
			}
		} break;
		case 0b00010: { // PRIMASK & CONTROL (exclusive)
			switch((uint8_t)bits<0, 3>::of(instruction.sysn)) {
				case 0b000: {
					val = special_regs.primask_register().pm();
				} break;
				case 0b100: {
					bits<0, 2>::of(val) = bits<0, 2>::of((uint32_t)special_regs.control_register());
				} break;
				default: {
					// unpredictable
					break;
				}
			}
		} break;
		default: {
			// unpredictable
			break;
		}
	}
	core_regs.set(instruction.rd, val);
}
static void exec(const msr instruction,
				 core_registers& regs,
				 special_registers& special_regs,
				 execution_mode& execution_mode) {
	switch(instruction.sysn) {
		case msr::SpecialRegister::APSR:
		case msr::SpecialRegister::IAPSR:
		case msr::SpecialRegister::EAPSR:
		case msr::SpecialRegister::XPSR: {
			// Note that the documentation says that the bits 27 to 31 should be copied.
			// However, the bit 27 is part of the reserved area of the APSR (and XPSR by
			// extension).
			// I suppose this is a documentation mistake and copy only the 4
			// APSR bits 28 to 31.
			special_regs.app_status_register().flags() =
				apsr_reg::flags_bits::of(regs.get(instruction.rn));
		} break;
		case msr::SpecialRegister::MSP: {
			// TODO: Should fail if not in privileged mode

			// align to word address (multiple of 4)
			uint32_t sp = binops::aligned<4>(regs.get(instruction.rn));
			regs.sp_register().set_specific_banked_sp(sp_reg::StackType::Main, sp);
		} break;
		case msr::SpecialRegister::PSP: {
			// TODO: Should fail if not in privileged mode

			// align to word address (multiple of 4)
			uint32_t sp = binops::aligned<4>(regs.get(instruction.rn));

			regs.sp_register().set_specific_banked_sp(sp_reg::StackType::Process, sp);
		} break;
		case msr::SpecialRegister::PRIMASK: {
			// TODO: MSR SpecialRegister::PRIMASK
			special_regs.primask_register().set_pm(bits<0>::of(regs.get(instruction.rn)));
		} break;
		case msr::SpecialRegister::CONTROL: {
			if(execution_mode::thread == execution_mode) {
				uint32_t val = regs.get(instruction.rn);
				special_regs.control_register().set_n_priv(bits<0>::of(val));
				special_regs.control_register().set_sp_sel(bits<1>::of(val));
			}
		} break;
		default: {
			// unpredictable
			break;
		}
	}
}
static void exec(const bl_imm instruction, core_registers& regs) {
	// pc is 4 bytes ahead, so already poiting to the next instruction
	uint32_t next_instr_addr = regs.get_pc();
	bits<0>::of(next_instr_addr) = true; // force thumb mode
	regs.set_lr(next_instr_addr);
	int32_t offset = instruction.offset();
	int32_t new_pc = regs.get_pc() + offset;
	regs.set_pc(new_pc);
}
static void exec(const dsb instruction) {}
static void exec(const dmb instruction) {}
static void exec(const isb instruction) {}
static void exec(const sev instruction) {}
static void exec(const wfi instruction) {
	/*
	 * If PRIMASK.PM is set to 1, an asynchronous exception that has a higher group priority than
	 * any active exception results in a WFI instruction exit. If the group priority of the
	 *exception is less than or equal to the execution group priority, the exception is ignored.
	 */
}
static void
exec(const wfe instruction, event_register& event_register, bool& enter_low_power_mode_signal) {
	// wait for event in event register

	/**
	 * The action of the Wait For Event instruction, see WFE on page A6-197, depends on the state of
	 * the Event Register:
	 * - If the Event Register is set, the instruction clears the register and returns immediately.
	 * - If the Event Register is clear the processor can suspend execution and enter a low-power
	 * state. It can remain in that state until the processor detects a WFE wakeup event or a reset.
	 * When the processor detects a WFE wakeup event, or earlier if the implementation chooses, the
	 * WFE instruction completes.
	 *
	 * The following events are WFE wake-up events:
	 * - the execution of an SEV instruction on any other processor in a multiprocessor system
	 * - any exception entering the pending state if SEVONPEND in the System Control Register is set
	 * to 1
	 * - an asynchronous exception at a priority that preempts any currently active exceptions
	 * - a debug event with debug enabled.
	 */

	if(event_register.is_set()) {
		event_register.clear();
	} else {
		// enter low power
		enter_low_power_mode_signal = true;
	}
}
static void exec(const yield instruction) {}
} // namespace micromachine::system
