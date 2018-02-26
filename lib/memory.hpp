//
// Created by fla on 5/1/17.
//

#ifndef MICROMACHINE_MEMORY_HPP
#define MICROMACHINE_MEMORY_HPP

#include <vector>
#include <algorithm>


#define memory_hardfault(reason_fmt,...)\
	fprintf(stderr, "memory hardfault: " reason_fmt, __VA_ARGS__); \
	_hardfault_signal = true; \


class memory {

public:


	struct mem_mapping {
		mem_mapping(uint8_t* host, uint32_t start_addr, uint32_t size)
			: host_mem(host)
			, range(start_addr, start_addr + size) {}

		const uint32_t& start() const {
			return range.first;
		}

		const uint32_t& end() const {
			return range.second;
		}

		void* translate(uint32_t addr) const {
			return host_mem + addr;
		}

		uint8_t* host_mem;
		std::pair<uint32_t, uint32_t> range;
	};


	memory(exception_vector::bitref_t& hardfault_signal)
		: _hardfault_signal(hardfault_signal) {

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


	void map(uint8_t* host, uint32_t start_addr, uint32_t size) {
		regions.emplace_back(host, start_addr, size);
	}

	template <typename access_t>
	static bool constexpr is_aligned(uint32_t address) {
		return 0 == (address & (sizeof(access_t)-1));
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

		if(!is_aligned<access_t>(address)) {
			memory_hardfault("unaligned memory access. write word at 0x%08X", address);
			return false;
		}

		mem_mapping* region = find_region(address);

		if(!region) {
			memory_hardfault("invalid memory access (unmapped) when writing word at 0x%08X", address);
			return false;
		}

		write<access_t>(region, address, value);

		return true;
	}

	template <typename access_t>
	access_t read(uint32_t address, bool& ok) const {
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

		const auto it = std::find_if(std::begin(regions), std::end(regions), [=](const mem_mapping& mm){
			return in_range(address, mm);
		});

		if(std::end(regions) != it) {
			return it.base();
		}

		return nullptr;
	}

	mem_mapping* find_region(uint32_t address) {
		return const_cast<mem_mapping*>(find_const_region(address));
	}

	region_vec regions;
	mutable exception_vector::bitref_t _hardfault_signal;

};

#endif //MICROMACHINE_MEMORY_HPP
