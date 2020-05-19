
//
// Created by fla on 14.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "peripherals/iodev.hpp"
#include "stream_connection.hpp"
#include "utils/waitable_condition.hpp"

#include <arpa/inet.h>
#include <atomic>
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <functional>
#include <future>
#include <list>
#include <map>
#include <sys/socket.h>
#include <sys/un.h>
#include <thread>
#include <unistd.h>
#include <unordered_set>
#include <utility>

namespace micromachine::system {

/**
 * Since the client list should be accessed by multiple threads, provide a thread safe client manager.
 */
class client_manager {
private:
	std::mutex _clients_mutex;
	waitable_condition _no_clients;
	waitable_condition _no_clients_to_delete;
	std::unordered_set<std::unique_ptr<stream_connection>> _clients_to_delete;
	std::unordered_map<stream_connection*, std::unique_ptr<stream_connection>> _clients;

public:
	client_manager()
		: _no_clients(true)
	{}

	~client_manager() {
		wait_no_more_clients();
		delete_removed_clients();
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

	void add_client(std::unique_ptr<stream_connection> client) {
		std::lock_guard<std::mutex> lock(_clients_mutex);
		_clients.emplace(client.get(), std::move(client));
		_no_clients.reset();
	}

	void delete_removed_clients() {
		std::lock_guard<std::mutex> lock(_clients_mutex);
		_clients_to_delete.clear();
		_no_clients_to_delete.set();

	}

	void wait_no_more_clients() {
		_no_clients.polling_wait(1ms);
	}

	void wait_no_more_clients_to_delete() {
		_no_clients_to_delete.polling_wait(1ms);
	}

	void remove_client(stream_connection& connection) {
		std::lock_guard<std::mutex> lock(_clients_mutex);
		auto found = _clients.find(&connection);
		if(found != _clients.end()) {
			_clients_to_delete.emplace(std::move(found->second));
			_no_clients_to_delete.reset();
			_clients.erase(found);
		}
		if(_clients.empty()) {
			_no_clients.set();
		}
	}
};

}
