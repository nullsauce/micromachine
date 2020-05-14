
//
// Created by fla on 14.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "peripherals/iodev.hpp"
#include "stream_connection.hpp"

#include <arpa/inet.h>
#include <atomic>
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <functional>
#include <future>
#include <list>
#include <map>
#include <unordered_set>
#include <sys/socket.h>
#include <sys/un.h>
#include <thread>
#include <unistd.h>
#include <utility>

namespace micromachine::system {

/**
 * Since the client list should be accessed by multiple threads, provide a thread safe client manager.
 */
class client_manager {
private:
	std::mutex _clients_mutex;
	std::unordered_map<stream_connection*, std::unique_ptr<stream_connection>> _clients;
	std::unordered_set<std::unique_ptr<stream_connection>> _clients_to_delete;

public:

	~client_manager() {
		clear();
	}

	void lock() {
		_clients_mutex.lock();
	}

	void unlock() {
		_clients_mutex.unlock();
	}

	auto begin() const {
		return _clients.begin();
	}

	auto end() const {
		return _clients.end();
	}

	size_t size() {
		std::lock_guard<std::mutex> lock(_clients_mutex);
		return _clients.size();
	}

	void add_client(std::unique_ptr<stream_connection>& client) {
		std::lock_guard<std::mutex> lock(_clients_mutex);
		_clients.emplace(client.get(), std::move(client));
	}

	void delete_removed_clients() {
		std::lock_guard<std::mutex> lock(_clients_mutex);
		_clients_to_delete.clear();
	}

	void remove_client(stream_connection& connection) {
		std::lock_guard<std::mutex> lock(_clients_mutex);
		auto found = _clients.find(&connection);
		if(found != _clients.end()) {
			_clients_to_delete.emplace(std::move(found->second));
			_clients.erase(found);
		}
	}

	void clear() {
		std::lock_guard<std::mutex> lock(_clients_mutex);
		for(auto& [_, client] : _clients) {
			client->close();
		}
		_clients.clear();
		_clients_to_delete.clear();
	}
};

}
