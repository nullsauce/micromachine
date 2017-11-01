//
// Created by fla on 5/1/17.
//

#ifndef MICROMACHINE_STATUS_FLAGS_HPP
#define MICROMACHINE_STATUS_FLAGS_HPP

#include "types.hpp"


struct apsr_register : public word {

	static const size_t FLAG_NEGATIVE 	= 31;
	static const size_t FLAG_ZERO 		= 30;
	static const size_t FLAG_CARRY 		= 29;
	static const size_t FLAG_OVERFLOW 	= 28;

	using word::integer_type;



	void print() {
		fprintf(stderr, "Z:%d, N:%d, C:%d, V:%d\n", zero_flag(), neg_flag(), carry_flag(), overflow_flag());
	}
	void apply_zero(const word& val) {
		write_zero_flag(val == 0);
	}

	void apply_neg(const word& val) {
		write_neg_flag(val.bit(31));
	}

	void write_neg_flag(bool status) {
		write_bit(FLAG_NEGATIVE, status);
	}

	bool neg_flag() const {
		return bit(FLAG_NEGATIVE);
	}

	void write_zero_flag(bool status) {
		write_bit(FLAG_ZERO, status);
	}

	bool zero_flag() const {
		return bit(FLAG_ZERO);
	}

	void write_carry_flag(bool status) {
		write_bit(FLAG_CARRY, status);
	}

	bool carry_flag() const {
		return bit(FLAG_CARRY);
	}

	void write_overflow_flag(bool status) {
		write_bit(FLAG_OVERFLOW, status);
	}

	bool overflow_flag() const {
		return bit(FLAG_OVERFLOW);
	}

	bool is_eq() const {
		return zero_flag();
	}

	bool is_ne() const {
		return !is_eq();
	}

	bool is_cs() const {
		return carry_flag();
	}

	bool is_cc() const {
		return !is_cs();
	}

	bool is_mi() const {
		return neg_flag();
	}

	bool is_pl() const {
		return !is_mi();
	}

	bool is_vs() const {
		return overflow_flag();
	}

	bool is_vc() const {
		return !is_vs();
	}

	bool is_hi() const {
		return carry_flag() && !zero_flag();
	}

	bool is_ls() const {
		return !is_hi();
	}

	bool is_ge() const {
		return neg_flag() == overflow_flag();
	}

	bool is_lt() const {
		return !is_ge();
	}

	bool is_gt() const {
		return (neg_flag() == overflow_flag()) && !zero_flag();
	}

	bool is_le() const {
		return !is_gt();
	}

	bool condition_passed(uint8_t condition) {
		switch(condition) {
			case 0b0000: return is_eq();
			case 0b0001: return is_ne();
			case 0b0010: return is_cs();
			case 0b0011: return is_cc();
			case 0b0100: return is_mi();
			case 0b0101: return is_pl();
			case 0b0110: return is_vs();
			case 0b0111: return is_vc();
			case 0b1000: return is_hi();
			case 0b1001: return is_ls();
			case 0b1010: return is_ge();
			case 0b1011: return is_lt();
			case 0b1100: return is_gt();
			case 0b1101: return is_le();
			default: return true;
		}

		return false;

		/*
		when '000' result = (APSR.Z == '1'); // EQ or NE
		when '001' result = (APSR.C == '1'); // CS or CC
		when '010' result = (APSR.N == '1'); // MI or PL
		when '011' result = (APSR.V == '1'); // VS or VC
		when '100' result = (APSR.C == '1') && (APSR.Z == '0'); // HI or LS
		when '101' result = (APSR.N == APSR.V); // GE or LT
		when '110' result = (APSR.N == APSR.V) && (APSR.Z == '0'); // GT or LE
		when '111' result = TRUE;*/
	}

};

#endif //MICROMACHINE_STATUS_FLAGS_HPP
