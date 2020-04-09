//
// Created by fla on 5/1/17.
//

#ifndef MICROMACHINE_MEMORY_HPP
#define MICROMACHINE_MEMORY_HPP

#include <vector>
#include <algorithm>
#include <unordered_map>
#include <type_traits>
#include <registers/system_control/systick.hpp>
#include "registers/system_control/shpr2.hpp"
#include "registers/system_control/shpr3.hpp"
#include "registers/system_control/systick.hpp"

#include "interrupter.hpp"
#include "mapping.hpp"


#define memory_hardfault(reason_fmt,...)\
	/*fprintf(stderr, "memory hardfault: " reason_fmt, __VA_ARGS__);*/ \
	_interrupter.raise_memory_hardfault();

class memory {
public:
	using region_vec = std::vector<memory_mapping>;
	using system_control_register_map = std::unordered_map<uint32_t, std::reference_wrapper<ireg>>;

	memory(interrupter& interrupter, const system_control_register_map& scr_map)
		: _interrupter(interrupter)
		, _system_control_registers(scr_map) {
	}

	bool write32(uint32_t address, uint32_t val) {
		return this->write<uint32_t>(address, val);
	}

	bool write16(uint32_t address, uint16_t val) {
		return this->write<uint16_t>(address, val);
	}

	bool write8(uint32_t address, uint8_t val) {
		return this->write<uint8_t>(address, val);
	}

	uint32_t read32(uint32_t address) const {
		return read<uint32_t>(address);
	}

	uint32_t read32(uint32_t address, bool& ok) const {
		return read<uint32_t>(address, ok);
	}

	uint16_t read16(uint32_t address) const {
		return read<uint16_t>(address);
	}

	uint16_t read16(uint32_t address, bool& ok) const {
		return read<uint16_t>(address, ok);
	}

	uint16_t read16_unchecked(uint32_t address) const {
		return read_unchecked<uint16_t>(address);
	}

	uint32_t read32_unchecked(uint32_t address) const {
		return read_unchecked<uint32_t>(address);
	}

	uint8_t read8(uint32_t address) const {
		return read<uint8_t>(address);
	}

	uint8_t read8(uint32_t address, bool& ok) const {
		return read<uint8_t>(address, ok);
	}

	uint8_t read8_unchecked(uint32_t address) const {
		return read_unchecked<uint8_t>(address);
	}


	void map(uint8_t* host, uint32_t start_addr, uint32_t size, const std::string& name) {
		_regions.emplace_back(host, start_addr, size, name);
	}

	void map(uint8_t* host, uint32_t start_addr, uint32_t size) {
		_regions.emplace_back(host, start_addr, size);
	}

	template <typename access_t>
	static bool constexpr is_aligned(uint32_t address) {
		return 0 == (address & (sizeof(access_t)-1));
	}

	const region_vec& regions() const {
		return _regions;
	}

	const memory_mapping* find_const_region(uint32_t address) const {
		const auto it = std::find_if(std::begin(_regions), std::end(_regions), [=](const memory_mapping& mm){
			return in_range(address, mm);
		});

		if(std::end(_regions) != it) {
			return it.base();
		}
		return nullptr;
	}

private:

	template <typename access_t>
	static access_t& access_host(void* host_addr) {
		return *(static_cast<access_t*>(host_addr));
	}

	template <typename access_t>
	static void write(memory_mapping* region, uint32_t addr, access_t val) {
		access_host<access_t>(region->translate(addr)) = val;
	}

	template <typename access_t>
	static access_t read(const memory_mapping* region, uint32_t addr) {
		return access_host<access_t>(region->translate(addr));
	}

	template <typename access_t>
	bool write(uint32_t address, access_t value) {
		// Only check if this is a system control register
		// access when access_t is uint32_t
		// TODO: use static_if
		if(std::is_same<uint32_t, access_t>::value) {
			if(address >= 0xE0000000) {
				auto reg_it = _system_control_registers.find(address);
				if(_system_control_registers.end() != reg_it) {
					reg_it->second.get() = value;
					return true;
				}
			}
		}

		if(!is_aligned<access_t>(address)) {
			memory_hardfault("unaligned memory access. write word at 0x%08X\n", address);
			return false;
		}

		memory_mapping* region = find_region(address);

		if(!region) {
			memory_hardfault("invalid memory access (unmapped) when writing word at 0x%08X\n", address);
			return false;
		}

		write<access_t>(region, address, value);

		return true;
	}



	template <typename access_t>
	access_t read(uint32_t address, bool& ok) const {
		// Only check if this is a system control register
		// access when access_t is uint32_t
		// TODO: use static_if
		if(std::is_same<uint32_t, access_t>::value) {
			if(address >= 0xE0000000) {
				auto reg_it = _system_control_registers.find(address);
				if(_system_control_registers.end() != reg_it) {
					return reg_it->second.get();
				}
			}
		}

		if(!is_aligned<access_t>(address)) {
			memory_hardfault("unaligned memory access. read word at 0x%08X\n", address);
			ok = false;
			return 0;
		}

		precond(is_aligned<access_t>(address),"unaligned memory access. read word at 0x%08X\n", address);
		const memory_mapping* region = find_const_region(address);

		if(!region) {
			memory_hardfault("invalid memory access (unmapped) when reading word at 0x%08X\n", address);
			ok = false;
			return 0;
		}
		ok = true;
		return read<access_t>(region, address);
	}

	template <typename access_t>
	access_t read_unchecked(uint32_t address) const {
		const memory_mapping* region = find_const_region(address);
		if(!region) {
			return 0;
		} else {
			return read<access_t>(region, address);
		}
	}

	template <typename access_t>
	access_t read(uint32_t address) const {
		bool dontcare;
		return read<access_t>(address, dontcare);
	}

	static bool in_range(uint32_t address, const memory_mapping& mm) {
		return address >= mm.start() && address < mm.end();
	}

	memory_mapping* find_region(uint32_t address) {
		return const_cast<memory_mapping*>(find_const_region(address));
	}

private:
	region_vec _regions;
	interrupter& _interrupter;
	const std::unordered_map<uint32_t, std::reference_wrapper<ireg>> _system_control_registers;

};

#endif //MICROMACHINE_MEMORY_HPP
