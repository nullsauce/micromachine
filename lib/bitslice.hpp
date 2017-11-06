#ifndef MICROMACHINE_BITSLICE_HPP
#define MICROMACHINE_BITSLICE_HPP


// Necessary for different slices to access each other _val event if their
// template parameters are different without having public val methods.
// TODO: make sure this is a zero cost abstraction
template<typename u_type>
struct integer_ref_holder {

	integer_ref_holder(u_type& val) : _val(val) {}

protected:

	u_type& val() {
		return _val;
	}

	const u_type& val() const {
		return _val;
	}

	u_type& _val;
};

template<typename u_type, size_t offset, size_t len>
struct bitslice : public integer_ref_holder<u_type> {

	static_assert(len > 0, "cannot declare a bit slice of length 0");
	static_assert(offset < binops::binsize<u_type>(),
			"source offset must be smaller than the total number of available bits");
	static_assert(len <= binops::binsize<u_type>(),
			"number of bits cannot be greater than the total number of available bits");
	static_assert(offset+len <= binops::binsize<u_type>(),
			"the binary slice cannot cover more than the total number of available bits");

	// use this inherited methods
	using integer_ref_holder<u_type>::integer_ref_holder;
	using integer_ref_holder<u_type>::val;
	using integer_ref_holder<u_type>::_val;

	using type = bitslice<u_type, offset, len>;

	template<size_t other_offset>
	type& operator= (const bitslice<u_type, other_offset, len>& other) {
		static_assert(other_offset < binops::binsize<u_type>(),
			"source offset must be smaller than the total number of bits");
		write_bits(_val, offset, other.val(), other_offset, len);
		return *this;
	}

	type& operator= (const type& other) {
		write_bits(_val, offset, other.val(), offset, len);
		return *this;
	}

	type& operator= (const u_type& other) {
		write_bits(_val, offset, other, 0, len);
		return *this;
	}

	template<size_t local_offset, size_t new_len>
	bitslice<u_type, offset+local_offset, new_len> bits() {
		return bitslice<u_type, offset+local_offset, new_len>(_val);
	};

	operator u_type() const {
		return extract();
	}

	u_type extract() const {
		return (_val >> offset) & binops::make_mask<u_type>(len);
	}

private:

	void write_bits(u_type& dst, size_t dst_offset, u_type src, size_t src_offset, size_t dst_len) {
		u_type to_place = binops::rlshift(src, src_offset, dst_offset);
		u_type mask = ~(binops::make_mask<u_type>(dst_len) << dst_offset);
		dst = (dst & (mask)) | (to_place & ~mask);
	}
};

#endif //MICROMACHINE_BITSLICE_HPP
