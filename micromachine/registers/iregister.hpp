#ifndef MICROMACHINE_IREGISTER_HPP
#define MICROMACHINE_IREGISTER_HPP

#include "types.hpp"

namespace micromachine::system {

/**
 * An object that can be converted to and from uint32_t and reset to
 * an initial value.
 */
class iregister {
public:
	iregister& operator=(uint32_t word) {
		set(word);
		return *this;
	}

	operator uint32_t() const {
		return get();
	}

	virtual void reset() = 0;

private:
	virtual void set(uint32_t word) = 0;
	virtual uint32_t get() const = 0;

};

} // namespace micromachine::system

#endif // MICROMACHINE_IREGISTER_HPP
