
//
// Created by fla on 04.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include <unordered_map>
#include <optional>

#include "registers/memory_mapped_reg.hpp"

namespace micromachine::system {

class register_map {
public:
	using map = std::unordered_map<uint32_t, std::reference_wrapper<iregister>>;

private:
	const map _map;
	const uint32_t _keys_hash;

public:
	register_map(map map)
		: _map(std::move(map))
		, _keys_hash(compute_all_keys_hash(_map))
	{}

	std::optional<std::reference_wrapper<iregister>> get_register_at(uint32_t address) const {
		if(is_not_a_mapped_address(address)) {
			return std::nullopt;
		}

		auto entry = _map.find(address);
		if(entry != _map.end()) {
			return entry->second;
		}

		return std::nullopt;
	}

private:
	/**
	 * Cpmpute a hash by ORing all they addresses together
	 * @param map
	 * @return
	 */
	static uint32_t compute_all_keys_hash(const map& map) {
		uint32_t hash = 0;
		for(const auto& [key, value] : map) {
			hash |= key;
		}
		return hash;
	}

	/**
	 * Check if the address is not a mapped address
	 * @param address
	 * @return if the address is definitely not in the set of mapped addresses
	 */
	bool is_not_a_mapped_address(uint32_t address) const {
		return ((address & _keys_hash) != address);
	}
};

}
