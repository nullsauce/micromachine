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
#include "registers/system_control/sphr2.hpp"
#include "registers/system_control/sphr3.hpp"
#include "registers/system_control/systick.hpp"
#include "exception_vector.hpp"

#define memory_hardfault(reason_fmt,...)\
	fprintf(stderr, "memory hardfault: " reason_fmt, __VA_ARGS__); \
    _exception.raise(exception_number::name::HARDFAULT);

namespace {

	template<class ForwardIt, class T, class Compare>
	ForwardIt lamda_upper_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp) {
		ForwardIt it;
		typename std::iterator_traits<ForwardIt>::difference_type count, step;
		count = std::distance(first,last);

		while (count > 0) {
			it = first;
			step = count / 2;
			std::advance(it, step);
			if (!comp(value, *it)) {
				first = ++it;
				count -= step + 1;
			} else count = step;
		}
		return first;
	}

	constexpr bool address_in_region(uint32_t address, uint32_t region_start, uint32_t region_end) {
		return (address >= region_start && address <= region_end);
	}
}

class memory {

public:

	using system_control_register_map = std::unordered_map<uint32_t, std::reference_wrapper<ireg>>;

	struct mem_mapping {
		mem_mapping(uint8_t* host_ptr, uint32_t start_addr, uint32_t size, const std::string& name = "")
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

		const uint8_t* host_mem() const {
			return _host_mem;
		}

		void* translate(uint32_t address) const {
			if(address < start()) {
				fprintf(stderr,"invalid address 0x%08X", address);
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

	memory(exception_vector& exception_vector, const system_control_register_map& scr_map)
		: _exception(exception_vector)
		, _system_control_registers(scr_map) {
	}

	using region_vec = std::vector<mem_mapping>;


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

	uint8_t read8(uint32_t address) const {
		return read<uint8_t>(address);
	}

	uint8_t read8(uint32_t address, bool& ok) const {
		return read<uint8_t>(address, ok);
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

private:



	template <typename access_t>
	static access_t& access_host(void* host_addr) {
		return *(static_cast<access_t*>(host_addr));
	}

	template <typename access_t>
	static void write(mem_mapping* region, uint32_t addr, access_t val) {
		access_host<access_t>(region->translate(addr)) = val;
	}

	template <typename access_t>
	static access_t read(const mem_mapping* region, uint32_t addr) {
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

		mem_mapping* region = find_region(address);

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
		const mem_mapping* region = find_const_region(address);

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
		const mem_mapping* region = find_const_region(address);
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

	static bool in_range(uint32_t address, const mem_mapping& mm) {
		return address >= mm.start() && address < mm.end();
	}

	const mem_mapping* find_const_region(uint32_t address) const {
		/*
		// System memory region,  - 0xFFFFFFFF
		// Check if within sub regions
		if(address >= 0xE0000000) {
			auto reg_it = _system_control_registers.find(address);
			if(_system_control_registers.end() != reg_it) {
				return reg_it;
			}
			// 1 Private Peripheral Bus (PPB) region (0xE0000000 - 0xE00FFFFF)
			if(address_in_region(address, 0xE0000000 , 0xE00FFFFF)) {
				// 1.1 System Control Space region (0xE000E000 - 0xE000EFFF)
				if(address_in_region(address, 0xE000E000, 0xE000EFFF)) {
					// 1.1.1 Auxiliary Control register (0xE000E000 - 0xE000E00F)
				} else if(address_in_region(address, 0xE000ED00, 0xE000ED8F)) {
					// 1.1.2 System Control Block (0xE000ED00 - 0xE000ED8F)
				} else if(address_in_region(address, 0xE000EF90, 0xE000EFCF)) {
					// 1.1.3 IMPLEMENTATION DEFINED (0xE000EF90 - 0xE000EFCF)
				} else if(address_in_region(address, 0xE000E010, 0xE000E0FF)) {
					// 1.1.4 Systick (0xE000E010 - 0xE000E0FF)
				} else if(address_in_region(address, 0xE000E100, 0xE000ECFF)) {
					// 1.1.5 NVIC (0xE000E100 - 0xE000ECFF)
				} else if(address_in_region(address, 0xE000EDF0, 0xE000EEFF)) {
					// 1.1.6 Debug control and configuration (0xE000EDF0 - 0xE000EEFF)
				} else if(address_in_region(address, 0xE000ED90, 0xE000EDEF)) {
					// 1.1.7 MPU (0xE000ED90 - 0xE000EDEF)
				}

			}
		} else if(0xE0100000 == address & 0xFFF00000) {
			// Vendor Sys region (0xE0100000 - 0xFFFFFFFF)

		}*/
#if 1
		const auto it = std::find_if(std::begin(_regions), std::end(_regions), [=](const mem_mapping& mm){
			return in_range(address, mm);
		});

		if(std::end(_regions) != it) {
			return it.base();
		}
		return nullptr;
#else
		auto found_region = std::upper_bound(_regions.begin(), _regions.end(), address, [](uint32_t search, const
		mem_mapping&
		region) {
			return  search < region.end();
		});
		if(_regions.end() == found_region || address >= found_region->end()) {
			return nullptr;
		}
		return found_region.base();

		/*
		auto found_region = lamda_upper_bound(std::begin(_regions), std::end(_regions), address, [] (const uint32_t
		search, const mem_mapping& region) {
			return region.end() > search;
		});

		// if the interator is valid, then check the address
		// is within the region size
		if(std::end(_regions) != found_region && found_region->start() < address) {
			return found_region.base();
		} else {
			return nullptr;
		}*/
#endif
	}



	mem_mapping* find_region(uint32_t address) {
		return const_cast<mem_mapping*>(find_const_region(address));
	}

    region_vec _regions;
	exception_vector& _exception;
	const std::unordered_map<uint32_t, std::reference_wrapper<ireg>> _system_control_registers;

};

#endif //MICROMACHINE_MEMORY_HPP
