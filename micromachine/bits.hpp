/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include <functional>
#include "binops.hpp"

namespace micromachine::system {
template<size_t offset, size_t len, typename integer_type>
struct slice {

	slice(integer_type& val)
		: _val(val) {}


	static_assert(len > 0,
				  "cannot declare a bit slice of length 0");
	static_assert(offset < binops::binsize<integer_type>(),
				  "source offset must be smaller than the total number of available bits");
	static_assert(offset + len <= binops::binsize<integer_type>(),
				  "the binary slice cannot cover more than the total number of available bits");


	using same_type = slice<offset, len, integer_type>;
	using const_type = slice<offset, len, const integer_type>;

	template<typename other_integer_type>
	using enable_only_if_const_differ = typename std::enable_if<!std::is_same<integer_type, other_integer_type>::value, same_type>::type;

	template<typename other_integer_type>
	using enable_only_if_source_has_less_or_equal_bits = typename std::enable_if<
		binops::binsize<other_integer_type>() <= len,
	same_type>::type;

	template<typename other_integer_type>
	using enable_only_if_source_has_more_bits = typename std::enable_if<(binops::binsize<other_integer_type>() > len), same_type>::type;

	using smallest_std_integer = typename std::conditional<len == 1,
		bool,
		typename std::conditional<len <= 8,
			uint8_t,
			typename std::conditional<len <= 16,
				uint16_t,
				typename std::conditional<len <= 32,
					uint32_t,
					uint64_t
				>::type
			>::type
		>::type
	>::type;

	// Operator for assignment of same slice  length and offset
	// bits<4,2> of A = bits<4,2> of B
	same_type& operator=(const same_type& other) {
		write_bits(_val, offset, other.val(), offset, len);
		return *this;
	}

	// A copy constructor must be declared explicitly
	slice(const same_type& other) : _val(other._val.get()) {}

	// Operator for assignment of same slice  length and offset but different constness of integer type
	// bits<4,2> of A = bits<4,2> of (0xdeabeef)
	// const version.
	template<typename other_constness_type>
	enable_only_if_const_differ<other_constness_type>&
	operator=(const slice<offset, len, other_constness_type>& other) {
		write_bits(_val, offset, other.val(), offset, len);
		return *this;
	}

	// Operator for assignment same slice length, but different offset
	// bits<4,2> of A = bits<6,2> of B
	template<size_t other_offset>
	same_type& operator=(const slice<other_offset, len, integer_type>& other) {
		static_assert(other_offset < binops::binsize<integer_type>(),
					  "source offset must be smaller than the total number of available bits");
		write_bits(_val, offset, other.val(), other_offset, len);
		return *this;
	}

	// Operator for assignment same slice length, but different offset and constness
	// bits<4,2> of A = bits<6,2> of B
	template<size_t other_offset, typename other_constness_type>
	enable_only_if_const_differ<other_constness_type>&
	operator=(const slice<other_offset, len, other_constness_type>& other) {
		static_assert(other_offset < binops::binsize<integer_type>(),
					  "source offset must be smaller than the total number of available bits");
		write_bits(_val, offset, other.val(), other_offset, len);
		return *this;
	}

	// Operator for assignment of different slice length and offset
	// bits<0,8> of A = bits<4,4> of B
	// extension by zero-filling
	template<size_t other_offset, size_t other_len>
	same_type& operator=(const slice<other_offset, other_len, integer_type>& other) {
		static_assert(other_len <= len,
					  "this slice can't hold that much bits");
		// clear bits<0,8>
		clear_bits(_val, offset, len);
		// write bits<4,4> of  to bits<0,4>
		write_bits(_val, offset, other.val(), other_offset, other_len);
		return *this;
	}

	// Allow conversion to bool if length equals 1
	template<size_t num_bits = len>
	operator typename std::enable_if<num_bits == 1, bool>::type() const {
		return extract();
	}

	// conversion always coalesced to smallest integer type
	operator smallest_std_integer() const {
		return extract();
	}

	smallest_std_integer extract() const {
		return (_val.get() >> offset) & binops::make_mask<integer_type>(len);
	}

	template<typename int_type, size_t num_bits = len>
	enable_only_if_source_has_less_or_equal_bits<int_type>& operator=(int_type other) {
		write_val(other);
		return *this;
	}

	template<typename int_type, size_t num_bits = len>
	enable_only_if_source_has_more_bits<int_type>& operator=(int_type other) {
		write_bits(_val, offset, other, 0, len);
		return *this;
	}

	integer_type& val() {
		return _val;
	}

	const integer_type& val() const {
		return _val;
	}

	void clear() {
		clear_bits(_val, offset, len);
	}

	template<size_t sub_offset, size_t new_len>
	slice<offset + sub_offset, new_len, integer_type> bits() {
		return slice<offset + sub_offset, new_len, integer_type>(_val);
	}

	template<typename T>
	bool operator==(const T& other) const {
		return extract() == other.extract();
	}

	bool are_set() const {
		return extract() == binops::make_mask<smallest_std_integer>(len);
	}


private:

	void write_bits(integer_type& dst, size_t dst_offset, integer_type src, size_t src_offset, size_t dst_len) {
		integer_type to_place = binops::rlshift(src, src_offset, dst_offset);
		integer_type mask = ~(binops::make_mask<integer_type>(dst_len) << dst_offset);
		dst = (dst & (mask)) | (to_place & ~mask);
	}

	void clear_bits(integer_type& dst, size_t dst_offset, size_t dst_len) {
		integer_type mask = ~(binops::make_mask<integer_type>(dst_len) << dst_offset);
		dst &= mask;
	}

	template<typename any_val>
	void write_val(any_val val) {
		static_assert(binops::binsize<any_val>() <= len,
					  "attempt to write more bits than what the destination can hold");
		write_bits(_val, offset, val, 0, len);
	}

	std::reference_wrapper<integer_type> _val;
};
template<size_t offset, size_t len = 1>
struct bits {

	template<typename integer_type>
	using integer_slice = slice<offset, len, integer_type>;

	template<typename integer_type>
	using const_integer_slice = typename integer_slice<integer_type>::const_type;

	template<typename integer_type>
	static const_integer_slice<integer_type> of(const integer_type& integer) {
		return const_integer_slice<integer_type>(integer);
	}

	template<typename integer_type>
	static integer_slice<integer_type> of(integer_type& integer) {
		return integer_slice<integer_type>(integer);
	}

	template<typename integer_type>
	static integer_type as_mask() {
		return binops::make_mask<integer_type>(len) << offset;
	}

	template<typename integer_type, size_t slice_offset, size_t slice_len>
	static slice<offset + slice_offset, len, integer_type>
	of(slice<slice_offset, slice_len, integer_type> existing_slice) {
		static_assert(offset + slice_offset + len <= slice_offset + slice_len,
					  "can't create a sub slice outside the parent slice bounds");
		return slice<offset + slice_offset, len, integer_type>(existing_slice.val());
	}
};

} // namespace micromachine::system
