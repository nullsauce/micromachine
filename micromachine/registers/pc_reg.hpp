#pragma once

#include "binops.hpp"
#include "exception/exception_return_handler.hpp"
#include "execution_mode.hpp"
#include "registers/epsr_reg.hpp"
#include "registers/standard_reg.hpp"
#include "standard_reg.hpp"

namespace micromachine::system {

class pc_reg : public standard_reg {
private:
	bool _dirty_status;

public:
	using standard_reg::standard_reg;
	using standard_reg::operator=;

	pc_reg()
		: _dirty_status(false) {}

	pc_reg(const pc_reg& existing_state)
		: standard_reg(existing_state)
		, _dirty_status(existing_state._dirty_status) {}

	void branch(uint32_t address) {
		set(address & 0xFFFFFFFE);
	}

	bool dirty_status() const {
		return _dirty_status;
	}

	void clear_dirty_status() {
		_dirty_status = false;
	}

private:
	static constexpr uint32_t VALUE_MASK = binops::make_mask<uint32_t>(1, 31);

	void set(uint32_t word) override {
		_word = word;
		_dirty_status = true;
	}

	uint32_t get() const override {
		return VALUE_MASK & _word;
	}
};

} // namespace micromachine::system
