
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
	using af_unix_connection_type = std::unique_ptr<stream_connection>;
	using connection_pair = std::pair<int, af_unix_connection_type>;
	std::mutex _clients_mutex;
	std::map<int, af_unix_connection_type> _clients;
	std::__cxx11::list<af_unix_connection_type> _clients_to_delete;

public:
	~client_manager() {
		clear();
	}

	size_t size() {
		std::lock_guard<std::mutex> lock(_clients_mutex);
		return _clients.size();
	}

	void add_new_client(connection_pair connection) {
		std::lock_guard<std::mutex> lock(_clients_mutex);
		_clients.emplace(std::move(connection));
	}

	void flush_delete_list() {
		std::lock_guard<std::mutex> lock(_clients_mutex);
		_clients_to_delete.clear();
	}

	void remove_client(stream_connection& connection) {
		std::lock_guard<std::mutex> lock(_clients_mutex);

		auto found = _clients.find(connection.socket());
		if(found != _clients.end()) {
			_clients_to_delete.push_back(std::move((*found).second));
			_clients.erase(found);
		}
	}

	void start(int socket) {
		std::lock_guard<std::mutex> lock(_clients_mutex);

		auto found = _clients.find(socket);
		if(found != _clients.end()) {
			found->second->start();
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

	void broadcast(uint8_t byte) {
		// send data to connected tcp clients
		std::lock_guard<std::mutex> lock(_clients_mutex);

		for(auto& [socket, client] : _clients) {
			client->send(byte);
		}
	}
};

}
