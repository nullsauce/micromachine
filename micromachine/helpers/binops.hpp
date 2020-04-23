#pragma once

#include "check.hpp"
#include <assert.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <string>

namespace micromachine::system::binops {

/**
 * Returns the number of bits used by type T
 * @tparam T integer type
 * @return The sie in bits of T
 */
template <typename T>
static constexpr size_t binsize() {
	return sizeof(T) * 8U;
}

/**
 * Overload for binsize<bool> that always returns 1
 * On most platform boolean are aligned to a byte
 * and will report a size of 1 byte.
 * @tparam T integer type
 * @return 1
 */
template <>
constexpr size_t binsize<bool>() {
	return 1U;
}

/**
 * Returns the number of bits used by type T (type deduced overload)
 * @tparam T integer type
 * @return
 */
template <typename T>
static constexpr size_t binsize(T) {
	return binsize<T>();
}

template <typename T>
static constexpr size_t get_sign_bit_index(T) {
	return binops::binsize<T>() - 1;
}

template <typename u_type>
static u_type rlshift(const u_type& source, size_t right_offset, size_t left_offset) {
	return (source >> right_offset) << left_offset;
}

template <typename u_type>
static bool get_bit(const u_type& source, size_t bit_offset) {
	micromachine_check(bit_offset < binsize<u_type>(), "offset is outside destination bits");
	return source & (1 << bit_offset);
}

template <typename u_type>
static bool get_sign_bit(const u_type& source) {
	return get_bit(source, get_sign_bit_index(source));
}
template <typename u_type>
static void set_bit(u_type& dest, size_t bit_offset) {
	micromachine_check(bit_offset < binsize<u_type>(), "offset is outside destination bits");
	dest = dest | (1U << bit_offset);
}

template <typename u_type>
void clear_bit(u_type& dest, size_t bit_offset) {
	micromachine_check(bit_offset < binsize<u_type>(), "offset is outside destination bits");
	dest = dest & ~(1U << bit_offset);
}

template <typename u_type>
static void write_bit(u_type& dest, size_t bit_offset, bool value) {
	micromachine_check(bit_offset < binsize<u_type>(), "offset is outside destination bits");
	if(value) {
		set_bit<u_type>(dest, bit_offset);
	} else {
		clear_bit<u_type>(dest, bit_offset);
	}
}

/**
 * Returns an integer of type u_type with the first num_bits lSB bits set to 1
 * @tparam u_type
 * @tparam num_bits
 * @return
 */
template <typename u_type>
static constexpr u_type make_mask(size_t num_bits) {
	return ((1ULL << num_bits) - 1ULL);
}

/**
 * Returns an integer of type u_type with all bits set to 1
 * @tparam u_type
 * @return
 */
template <typename u_type>
static constexpr u_type make_mask() {
	return make_mask<u_type>(binsize<u_type>());
}

/**
 * Returns an integer of type u_type with the bits from offset to offset + num_bits set to 1
 * @tparam u_type
 * @tparam num_bits
 * @return
 */
template <typename u_type>
static constexpr u_type make_mask(size_t offset, size_t num_bits) {
	return ((1ULL << num_bits) - 1ULL) << offset;
}

template <size_t _offset, size_t _size>
struct range {
	static const size_t offset = _offset;
	static const size_t size = _size;
};

template <typename u_type>
u_type sign(const u_type& source, const size_t num_bits) {
	const bool sign_bit = get_bit<u_type>(source, num_bits - 1U);
	const u_type kept_part_mask = make_mask<u_type>(num_bits - 1U);
	const u_type kept_part = source & kept_part_mask;
	const u_type extension = (~kept_part_mask) * sign_bit;
	return kept_part | extension;
}

template <typename u_type>
static u_type twos_complement(const u_type& source) {
	return ~source + 1;
}

template <typename u_type>
static u_type read_uint(const u_type& source, const size_t bit_offset, const size_t num_bits) {

	micromachine_check(bit_offset < binsize<u_type>(), "offset out of range");
	micromachine_check(num_bits <= binsize<u_type>(), "source value can't hold num_bits");
	micromachine_check(bit_offset < binsize<u_type>(), "offset is outside destination bits");
	micromachine_check((bit_offset + num_bits) <= binsize<u_type>(), "source is to small");

	return (source >> bit_offset) & make_mask<u_type>(num_bits);
}

template <typename u_type>
static u_type read_sint(const u_type& source, const size_t bit_offset, const size_t num_bits) {

	micromachine_check(num_bits > 1, "signed integer must have more than one bit");

	const u_type unsigned_val = read_uint<u_type>(source, bit_offset, num_bits);
	return binops::sign<u_type>(unsigned_val, num_bits);
}

template <typename u_type>
static bool all_bits_set(const u_type& source, size_t bit_offset, size_t num_bits) {
	return read_uint<u_type>(source, bit_offset, num_bits) == make_mask<u_type, num_bits>();
}

template <typename u_type>
static bool any_bits_set(const u_type& source, size_t bit_offset, size_t num_bits) {
	return 0 != read_uint<u_type>(source, bit_offset, num_bits);
}

template <typename u_type>
constexpr bool is_powerof2(u_type source) {
	auto lower_bits_mask = source - 1U;
	auto only_lower_bits = source & lower_bits_mask;
	return source && (only_lower_bits == 0U);
}

template <size_t align, typename u_type>
static u_type aligned(const u_type& source) {
	static_assert(is_powerof2(align), "Can't align on non power of two values");
	return (source / align) * align;
}

static constexpr uint64_t swap64(const uint64_t val) {
	return ((val & 0x00000000000000FFULL) << 56) | ((val & 0x000000000000FF00ULL) << 40) |
		   ((val & 0x0000000000FF0000ULL) << 24) | ((val & 0x00000000FF000000ULL) << 8) |
		   ((val & 0x000000FF00000000ULL) >> 8) | ((val & 0x0000FF0000000000ULL) >> 24) |
		   ((val & 0x00FF000000000000ULL) >> 40) | ((val & 0xFF00000000000000ULL) >> 56);
}

static constexpr uint32_t swap32(const uint32_t val) {
	return ((val & 0x000000ff) << 24) | ((val & 0x0000ff00) << 8) | ((val & 0x00ff0000) >> 8) |
		   ((val & 0xff000000) >> 24);
}

static constexpr uint16_t swap16(const uint16_t val) {
	return (val << 8) | (val >> 8);
}

template <typename u_type>
static u_type swap(const u_type& value) {
	switch(binsize<u_type>()) {
		case 8U:
			return value;
		case 16U:
			return swap16(value);
		case 32U:
			return swap32(value);
		case 64U:
			return swap64(value);
		default:
			micromachine_fail("No known swap conversion");
	}
}

template <typename u_type>
std::string to_string(const u_type& value) {
	const size_t binsize = binops::binsize<u_type>();
	std::string str(binsize, '0');
	for(size_t i = 0; i < binsize; i++) {
		if(get_bit(value, binsize - i - 1)) {
			str[i] = '1';
		}
	}
	return str;
}

} // namespace micromachine::system::binops
