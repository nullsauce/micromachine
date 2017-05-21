//
// Created by fla on 17-5-8.
//

#ifndef MICROMACHINE_INTEGER_TYPE_HPP
#define MICROMACHINE_INTEGER_TYPE_HPP


#include "binops.hpp"
#include <string>
#include <type_traits>

template <typename bits_type>
class integer_type {

public:


	using bits_t 	= bits_type;
	using sbits_t 	= typename std::make_signed<bits_t>::type;

	integer_type() : _val(0) {}

	integer_type(bits_t v)
			: _val(to_host_order(v))
	{}

	operator bits_t() const {
		return unpack();
	}

	bits_t uint(size_t bit_offset, size_t num_bits) const {
		return binops::read_uint<bits_t>(*this, bit_offset, num_bits);
	}

	sbits_t sint(size_t bit_offset, size_t num_bits) const {
		return binops::read_sint<bits_t>(*this, bit_offset, num_bits);
	}

	static bits_t make_mask(size_t num_bits) {
		return binops::make_mask<bits_t>(num_bits);
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

	bool bit(size_t offset) const {
		return binops::get_bit<bits_t>(*this, offset);
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


	std::string to_string() {
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

	static bits_t platform_to_host_order(bits_t val) {
		return swap(val);
	}

	static bits_t host_to_platform_order(bits_t val) {
		return swap(val);
	}

	static bits_t to_host_order(bits_t val) {
		if(is_big_endian()) {
			return platform_to_host_order(val);
		}
		return val;
	}

	bits_t unpack() const {
		return to_host_order(_val);
	};


	bits_t _val;

};

#endif //MICROMACHINE_INTEGER_TYPE_HPP
