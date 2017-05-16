// @micromachine_project_header@
// Created by roth on 04.05.17.
//

#ifndef MICROMACHINE_BINOPS_HPP
#define MICROMACHINE_BINOPS_HPP

#include <cstdint>
#include <cstdlib>
#include <cstdio>
#include <exception>
#include <assert.h>
#include "precond.hpp"


namespace binops {

template<typename T>
static constexpr size_t binsize() {
	return sizeof(T) * 8U;
}

template<typename T>
static constexpr size_t binsize(T val) {
	return binsize<T>();
}

template <typename u_type>
static bool get_bit(const u_type& source, size_t bit_offset) {
	precond(bit_offset < binsize<u_type>(), "offset is outside destination bits");
	return source & (1 << bit_offset);
}

template <typename u_type>
static void set_bit(u_type& dest, size_t bit_offset) {
	precond(bit_offset < binsize<u_type>(), "offset is outside destination bits");
	dest = dest | (1U << bit_offset);
}

template <typename u_type>
void clear_bit(u_type& dest, size_t bit_offset) {
	precond(bit_offset < binsize<u_type>(), "offset is outside destination bits");
	dest = dest & ~(1U << bit_offset);
}

template <typename u_type>
static void write_bit(u_type& dest, size_t bit_offset, bool value) {
	precond(bit_offset < binsize<u_type>(), "offset is outside destination bits");
	if(value) {
		set_bit<u_type>(dest, bit_offset);
	} else {
		clear_bit<u_type>(dest, bit_offset);
	}
}

template<size_t num_bits>
static constexpr size_t make_mask() {
	return ((1ULL<<num_bits)-1ULL);
}

template<size_t offset, size_t num_bits>
static constexpr size_t make_mask() {
	return make_mask<num_bits>() << offset;
}

template<typename u_type>
static u_type make_mask(const size_t num_bits) {
	precond(num_bits <= binsize<u_type>(), "dest value can't hold num_bits");
	return ((1ULL<<num_bits)-1ULL);
}

template<size_t _offset, size_t _size>
struct range {
	static const size_t offset = _offset;
	static const size_t size = _size;
};

template<typename u_type, typename bitrange>
static u_type genbits() {
	return make_mask<u_type>(bitrange::size) << bitrange::offset;
}

template<typename u_type, size_t offset, size_t size>
static u_type genbits() {
	return make_mask<u_type>(size) << offset;
}

template<typename u_type>
u_type sign(const u_type& source, const size_t num_bits) {
	const bool sign_bit = get_bit<u_type>(source, num_bits-1U);
	const u_type kept_part_mask = make_mask<u_type>(num_bits-1U);
	const u_type kept_part = source & kept_part_mask;
	const u_type extension = (~kept_part_mask) * sign_bit;
	return kept_part | extension;
}

template<typename u_type>
static u_type twos_complement(const u_type& source) {
	return ~source + 1;
}

template<typename u_type>
static u_type read_uint(const u_type& source, const size_t bit_offset, const size_t num_bits) {

	precond(bit_offset <  binsize<u_type>(), "offset out of range");
	precond(num_bits <= binsize<u_type>(), "source value can't hold num_bits");
	precond(bit_offset < binsize<u_type>(), "offset is outside destination bits");
	precond((bit_offset + num_bits) <= binsize<u_type>(), "source is to small");

	return (source >> bit_offset) & make_mask<u_type>(num_bits);
}

template<typename u_type>
static u_type read_sint(const u_type& source, const size_t bit_offset, const size_t num_bits) {

	precond(num_bits > 1, "signed integer must have more than one bit");

	const u_type unsigned_val = read_uint<u_type>(source, bit_offset, num_bits);
	return binops::sign<u_type>(unsigned_val, num_bits);
}

template <typename u_type>
static bool all_bits_set(const u_type& source, size_t bit_offset, size_t num_bits) {
	return read_uint<u_type>(source, bit_offset, num_bits) == make_mask<u_type>(num_bits);
}

template <typename u_type>
static bool any_bits_set(const u_type& source, size_t bit_offset, size_t num_bits) {
	return 0 != read_uint<u_type>(source, bit_offset, num_bits);
}

template <typename u_type>
static u_type aligned(const u_type& source, size_t align) {
	return (source / align) * align;
}


static constexpr uint64_t swap64(const uint64_t val) {
	return
			((val & 0x00000000000000FFULL) << 56) |
			((val & 0x000000000000FF00ULL) << 40) |
			((val & 0x0000000000FF0000ULL) << 24) |
			((val & 0x00000000FF000000ULL) <<  8) |
			((val & 0x000000FF00000000ULL) >>  8) |
			((val & 0x0000FF0000000000ULL) >> 24) |
			((val & 0x00FF000000000000ULL) >> 40) |
			((val & 0xFF00000000000000ULL) >> 56);
}

static constexpr uint32_t swap32(const uint32_t val) {
	return
			((val & 0x000000ff) << 24) |
			((val & 0x0000ff00) << 8)  |
			((val & 0x00ff0000) >> 8)  |
			((val & 0xff000000) >> 24);
}

static constexpr uint16_t swap16(const uint16_t val) {
	return     (val << 8) |
			   (val >> 8);
}

template<typename u_type>
static u_type swap(const u_type& value) {
	switch(binsize<u_type>()) {
		case 8U: return value;
		case 16U: return swap16(value);
		case 32U: return swap32(value);
		case 64U: return swap64(value);
		default: precond_fail("No known swap conversion");
	}
}

};

static constexpr bool is_big_endian() {
	return (*(uint16_t *)"\0\xff" < 0x100);
}



#endif //MICROMACHINE_BINOPS_HPP
