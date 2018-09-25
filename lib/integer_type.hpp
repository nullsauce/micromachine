//
// Created by fla on 17-5-8.
//

#ifndef MICROMACHINE_INTEGER_TYPE_HPP
#define MICROMACHINE_INTEGER_TYPE_HPP


#include "binops.hpp"
#include "bitslice.hpp"
#include <string>
#include <type_traits>
#include <iostream>

template <typename bits_type>
class integer_type {

public:


	using bits_t 	= bits_type;
	using sbits_t 	= typename std::make_signed<bits_t>::type;
	using u_type 	= integer_type<bits_type>;
	integer_type() : _val(0) {}

	integer_type(bits_t v)
			: _val(v)
	{}

	operator bits_t() const {
		return _val;
	}

	bits_t uint(size_t bit_offset, size_t num_bits) const {
		return binops::read_uint(*this, bit_offset, num_bits);
	}

	void set_bit(size_t offset) {
		binops::set_bit(*this, offset);
	}

	void clear_bit(size_t offset) {
		binops::clear_bit(*this, offset);
	}

	void write_bit(size_t offset, bool value) {
		binops::write_bit(*this, offset, value);
	}

	// TODO: Replace caller of this by binops
	void write_bits(size_t dst_offset, size_t src_offset, integer_type<bits_t> src, size_t num_bits) {
		auto to_place 	= binops::rlshift(src, src_offset, dst_offset);
		auto mask = binops::make_mask<bits_t>(num_bits) << dst_offset;
		_val = (_val & (~mask)) | to_place;
	}


	template<size_t offset, size_t len>
	bitslice<offset, len, u_type> bits() {
		return bitslice<offset, len, u_type>(*this);
	};

	template<size_t offset, size_t len>
	bitslice<offset, len, const u_type> bits() const {
		return bitslice<offset, len, const u_type>(*this);
	};

	bool bit(size_t offset) const {
		return binops::get_bit(*this, offset);
	}

	bool any_bits_set(size_t offset, size_t num_bits) const {
		return binops::any_bits_set(*this, 0, 2);
	}

	bool sign_bit() const {
		return bit(binops::binsize(*this)-1);
	}

	bits_t aligned(size_t size) const {
		return binops::aligned(*this, size);
	}


	std::string to_string() const {
		const size_t binsize = binops::binsize(*this);
		std::string str(binsize, '0');
		for(size_t i = 0; i < binsize; i++) {
			if(bit(binsize - i - 1)) {
				str[i] = '1';
			}
		}
		return str;
	}

private:

	static bits_t swap(bits_t val) {
		return binops::swap(val);
	}

	bits_t _val;

};

#endif //MICROMACHINE_INTEGER_TYPE_HPP
