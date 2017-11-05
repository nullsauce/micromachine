//
// Created by fla on 17-5-20.
//

#ifndef MICROMACHINE_EXCEPTION_VECTOR_HPP
#define MICROMACHINE_EXCEPTION_VECTOR_HPP

#include <array>
#include "exception.hpp"
#include "types.hpp"

template <typename u_type>
class bitref {

public:

	bitref(u_type& block, size_t offset)
		:_block(block)
		,_offset(offset) {

	}

	operator bool () const {
		return read();
	}

	bitref& operator = (bool value) {
		write(value);
		return *this;
	}

private:

	bool read() const {
		return binops::get_bit(_block, _offset);
	}

	void write(bool value) const {
		binops::write_bit(_block, _offset, value);
	}

	u_type& _block;
	const size_t _offset;
};

class exception_vector {

public:

	using storage_t = uint32_t;
	using bitref_t = bitref<storage_t>;
	static const size_t size = binops::binsize<storage_t>();

	exception_vector()
		: _storage(0)
		, _signals({
		   {_storage, 0},
		   {_storage, 1},
		   {_storage, 2},
		   {_storage, 3},
		   {_storage, 4},
		   {_storage, 5},
		   {_storage, 6},
		   {_storage, 7},
		   {_storage, 8},
		   {_storage, 9},
		   {_storage, 10},
		   {_storage, 11},
		   {_storage, 12},
		   {_storage, 13},
		   {_storage, 14},
		   {_storage, 15},
		   {_storage, 16},
		   {_storage, 17},
		   {_storage, 18},
		   {_storage, 19},
		   {_storage, 20},
		   {_storage, 21},
		   {_storage, 22},
		   {_storage, 23},
		   {_storage, 24},
		   {_storage, 25},
		   {_storage, 26},
		   {_storage, 27},
		   {_storage, 28},
		   {_storage, 29},
		   {_storage, 30},
		   {_storage, 31}
		}){
	}

	bitref_t& operator[](exception ex) {
		return _signals[(size_t)ex];
	}

	bool is_signaled(exception ex) const {
		return (bool)_signals[(size_t)ex];
	}

	bool any_signaled() const {
		return 0 != _storage;
	}

	const storage_t& storage() const {
		return _storage;
	}

	void clear() {
		_storage = 0;
	}
private:

	storage_t _storage;
	bitref_t _signals[size];
};

#endif //MICROMACHINE_EXCEPTION_VECTOR_HPP
