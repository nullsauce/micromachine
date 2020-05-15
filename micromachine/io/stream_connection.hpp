
//
// Created by fla on 14.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "helpers/check.hpp"
#include "io/helpers.hpp"
#include "peripherals/iodev.hpp"

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

class stream_connection {
public:
	using client_disconnect_callback_t = std::function<void(stream_connection& connection)>;
	using new_data_callback_t = std::function<void(const uint8_t* buffer, size_t size, void* user_param)>;

private:
	const std::string& _unix_domain_socket;
	int _socket;

	/**
	 * Called whenever a connection has been disconnected.
	 */
	client_disconnect_callback_t _disconnection_callback;

	/**
	 * Called whenever a new data has been received from the connection
	 */
	new_data_callback_t _new_data_callback;

	/**
	 * Generic user param associated with _new_data_callback
	 */
	void* _user_param;


	std::atomic<bool> _listener_is_running;

	/**
	 * Waitable signal for starting _listener_thread
	 */
	std::promise<void> _listener_has_stared;

	/**
	 * Waitable signal for stopping _listener_thread
	 */
	std::promise<void> _listener_has_stopped;
	std::thread _listener_thread;

public:
	// create stream_connection from a connected socket
	stream_connection(const int socket,
					  const std::string& unix_domain_socket,
					  client_disconnect_callback_t disconnection_callback = nullptr,
					  new_data_callback_t new_data_callback = nullptr,
					  void* user_param = nullptr)
		: _unix_domain_socket(unix_domain_socket)
		, _socket(socket)
		, _disconnection_callback(std::move(disconnection_callback))
		, _new_data_callback(std::move(new_data_callback))
		, _user_param(user_param)
		, _listener_is_running(false) {}

	explicit stream_connection(const std::string& unix_domain_socket,
							   client_disconnect_callback_t disconnection_callback = nullptr,
							   new_data_callback_t new_data_callback = nullptr,
							   void* user_param = nullptr)

		: _unix_domain_socket(unix_domain_socket)
		, _socket(create_and_connect_socket(_unix_domain_socket))
		, _disconnection_callback(std::move(disconnection_callback))
		, _new_data_callback(std::move(new_data_callback))
		, _user_param(user_param)
		, _listener_is_running(true)
		, _listener_thread(std::thread(&stream_connection::listener, this)) {
		bool timeout = wait_signal(_listener_has_stared, 500);
		if(timeout) {
			_listener_is_running = false;
			if(_listener_thread.joinable()) {
				_listener_thread.join();
			}
			throw std::runtime_error("signal cannot be catch on time");
		}
	}

	~stream_connection() {
		close();
	}

	void start() {
		_listener_thread = std::thread(&stream_connection::listener, this);
		bool timeout = wait_signal(_listener_has_stared, 500);
		if(timeout) {
			_listener_is_running = false;
			if(_listener_thread.joinable()) {
				_listener_thread.join();
			}
			throw std::runtime_error("signal cannot be catch on time");
		}
	}
	}

	const int& socket() const {
		return _socket;
	}

	int close() {


		shutdown(_socket, SHUT_RDWR);
		int r = ::close(_socket);

		_listener_is_running = false;

		if(_listener_thread.joinable()) {
			_listener_thread.join();
		}

		return r;
	}
	void send(uint8_t byte) const {

		ssize_t size = ::send(_socket, &byte, 1, 0);
		if(size == 0) {
			fprintf(stderr, "[%s] send size is 0!\n", __PRETTY_FUNCTION__);
			return;
		} else if(size == -1) {
			fprintf(stderr, "[%s] send size is -1!\n", __PRETTY_FUNCTION__);
			perror("send failed!");
			return;
		}
	}

	ssize_t send(const uint8_t* buffer, size_t size) const {
		return ::send(_socket, buffer, size, 0);
	}

private:
	static int create_and_connect_socket(const std::string& unix_domain_socket) {

		int socket = ::socket(AF_UNIX, SOCK_STREAM, 0);
		if(socket == -1) {
			throw std::runtime_error("failed to create client socket: " + std::string(strerror(errno)));
		}

		struct sockaddr_un server_address = {};
		server_address.sun_family = AF_UNIX;

		strcpy(server_address.sun_path, unix_domain_socket.c_str());

		int connect_status = connect(socket, (const struct sockaddr*)&server_address, sizeof(server_address));
		if(connect_status) {
			throw std::runtime_error("failed to create connect: " + std::string(strerror(errno)));
		}

		return socket;
	}

	/**
	 * Process received stream from the socket.
	 * Dispatch disconnection and new data events.
	 */
	void listener() {

		static constexpr size_t len = 256;
		char buffer[len] = {};

		// ctor synchronization
		_listener_is_running = true;
		_listener_has_stared.set_value();

		while(_listener_is_running) {

			memset(buffer, 0, len);
			ssize_t received = recv(_socket, buffer, len, 0);
			if(received == -1) {
				break;
			} else if(received == 0) {
				if(_disconnection_callback) {
					_disconnection_callback(*this);
				}
				break;
			}

			if(_new_data_callback) {
				if(_user_param == nullptr) {
					_new_data_callback((const uint8_t*)buffer, received, this);
				} else {
					_new_data_callback((const uint8_t*)buffer, received, _user_param);
				}
			}
		}

		// dtor synchronization
		_listener_has_stopped.set_value();
		_listener_is_running = false;
	}
};

}
