
//
// Created by fla on 04.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "registers/standard_reg.hpp"
#include "types/types.hpp"

namespace micromachine::system {

class vtable_offset_reg : public standard_reg {
public:
	static constexpr uint32_t VTOR = 0xE000ED08;

private:
	void set(uint32_t word) override {
		// writes to this register are ignored
	}

	uint32_t get() const override {
		return 0U;
	}
};

} // namespace micromachine::system
