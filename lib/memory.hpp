//
// Created by fla on 5/1/17.
//

#ifndef MICROMACHINE_MEMORY_HPP
#define MICROMACHINE_MEMORY_HPP

#include <vector>
#include <algorithm>


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

	using region_vec = std::vector<mem_mapping>;


	void write32(uint32_t address, uint32_t val) {
		this->write<uint32_t>(address, val);
	}

	void write16(uint32_t address, uint16_t val) {
		this->write<uint16_t>(address, val);
	}

	void write8(uint32_t address, uint8_t val) {
		this->write<uint8_t>(address, val);
	}

	uint32_t read32(uint32_t address) const {
		return read<uint32_t>(address);
	}

	uint16_t read16(uint32_t address) const {
		return read<uint16_t>(address);
	}

	uint8_t read8(uint32_t address) const {
		return read<uint8_t>(address);
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
	void write(uint32_t address, access_t value) {
		precond(is_aligned<access_t>(address),"unaligned memory access. write word at 0x%08X", address);
		mem_mapping* region = find_region(address);
		precond(region, "invalid memory access (unmapped) when writing word at 0x%08X", address);
		write<access_t>(region, address, value);
	}

	template <typename access_t>
	access_t read(uint32_t address) const {
		precond(is_aligned<access_t>(address),"unaligned memory access. read word at 0x%08X", address);
		const mem_mapping* region = find_const_region(address);
		precond(region, "invalid memory access (unmapped) when reading word at 0x%08X", address);
		return read<access_t>(region, address);
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

	/*
	region_vec find_regions(uint32_t address) const {
		std::find_if(regions.begin(), regions.end(), [=](const mem_mapping& mm){
			return in_range(address, mm);
		});
	}*/

	region_vec regions;

};

#endif //MICROMACHINE_MEMORY_HPP
