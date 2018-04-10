#ifndef MICROMACHINE_BITSLICE_HPP
#define MICROMACHINE_BITSLICE_HPP

#include <functional>

template<size_t offset, size_t len, typename u_type>
struct bitslice /*: public integer_ref_holder<u_type>*/ {

	bitslice(u_type& val) : _val(val) {}

	static_assert(len > 0, "cannot declare a bit slice of length 0");
	static_assert(offset < binops::binsize<u_type>(),
			"source offset must be smaller than the total number of available bits");
	static_assert(len <= binops::binsize<u_type>(),
			"number of bits cannot be greater than the total number of available bits");
	static_assert(offset+len <= binops::binsize<u_type>(),
			"the binary slice cannot cover more than the total number of available bits");

	using type = bitslice<offset, len, u_type>;

	template<size_t other_offset>
	type& operator= (const bitslice<other_offset, len, const u_type>& other) {
		static_assert(other_offset < binops::binsize<u_type>(),
			"source offset must be smaller than the total number of bits");
		write_bits(_val, offset, other.val(), other_offset, len);
		return *this;
	}

	type& operator= (const u_type& other) {
		write_bits(_val, offset, other, 0, len);
		return *this;
	}

	operator u_type() const {
		return extract();
	}

	u_type extract() const {
		return (_val.get() >> offset) & binops::make_mask<u_type>(len);
	}

	std::reference_wrapper<u_type> val() {
		return _val;
	}

	const std::reference_wrapper<u_type> val() const {
		return _val;
	}

private:

	void write_bits(u_type& dst, size_t dst_offset, u_type src, size_t src_offset, size_t dst_len) {
		u_type to_place = binops::rlshift(src, src_offset, dst_offset);
		u_type mask = ~(binops::make_mask<u_type>(dst_len) << dst_offset);
		dst = (dst & (mask)) | (to_place & ~mask);
	}

	std::reference_wrapper<u_type> _val;
};

template<size_t offset, size_t len>
struct bits {

	template<typename u_type>
	static bitslice<offset, len, u_type> of(u_type& integer) {
		return bitslice<offset, len, u_type>(integer);
	}

	template<typename u_type>
	static const bitslice<offset, len, const u_type> of(const u_type& integer) {
		return bitslice<offset, len, const u_type>(integer);
	}
};



#endif //MICROMACHINE_BITSLICE_HPP
