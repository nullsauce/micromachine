/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_MEM_MAPPING_HPP
#define MICROMACHINE_EMU_MEM_MAPPING_HPP

#include <string>

namespace micromachine::system {
class memory_mapping {
public:
	memory_mapping(uint8_t* host_ptr, uint32_t start_addr, uint32_t size, const std::string& name = "")
		: _host_mem(host_ptr)
		, _start(start_addr)
		, _end(start_addr + size)
		, _name(name) {}

	uint32_t start() const {
		return _start;
	}

	uint32_t end() const {
		return _end;
	}

	uint32_t size() const {
		return end() - start();
	}

	const uint8_t* host_mem() const {
		return _host_mem;
	}

	uint8_t* host_mem() {
		return _host_mem;
	}

	void* translate(uint32_t address) const {
		if(address < start()) {
			fprintf(stderr,"invalid address 0x%08X\n", address);
			return nullptr;
		}
		return _host_mem + (address - start());
	}

	const std::string& name() const {
		return _name;
	}

	uint8_t* const _host_mem;
	const uint32_t _start;
	const uint32_t _end;
	const std::string _name;
};
} // namespace micromachine::system

#endif //MICROMACHINE_EMU_MEM_MAPPING_HPP
