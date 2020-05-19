
//
// Created by fla on 14.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "helpers/check.hpp"
#include "peripherals/iodev.hpp"
#include "utils/waitable_condition.hpp"

#include <atomic>
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <functional>
#include <list>
#include <map>
#include <thread>
#include <utility>
#include <chrono>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

namespace micromachine::system {

using namespace std::chrono_literals;

class stream_connection {
public:
	using client_disconnect_callback_t = std::function<void(stream_connection& connection)>;
	using new_data_callback_t = std::function<void(const uint8_t* buffer, size_t size, void* user_param)>;

private:
	const int _socket;

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

	/**
	 * Flag set by the main thread that indicates to the client thread that it should stop
	 * read operations and exit.
	 */
	std::atomic<bool> _shutdown_requested;


	/**
	 * Flag set when the client receive loop ended.
	 */
	waitable_condition _disconnected;

	/**
	 * Waitable signal set by the client thread that indicates the thread has successfully started.
	 * This signal is used to ensure the client constructor returns only after its thread has started.
	 */
	waitable_condition _listener_thread_ready;

	/**
	 * Waitable signal set by the main thread that indicates to the client that it can start
	 * reading the socket. The client thread will wait indefinitely for this signal after
	 * initialization.
	 */
	waitable_condition _start_reading;

	/**
	 * The client thread. Always started by the constructor.
	 */
	std::thread _listener_thread;

public:
	// create stream_connection from a connected socket
	stream_connection(const int socket,
					  client_disconnect_callback_t disconnection_callback = nullptr,
					  new_data_callback_t new_data_callback = nullptr,
					  void* user_param = nullptr)
		: _socket(socket)
		, _disconnection_callback(std::move(disconnection_callback))
		, _new_data_callback(std::move(new_data_callback))
		, _user_param(user_param)
		, _shutdown_requested(false)
		, _disconnected(false)
		, _listener_thread(std::thread(&stream_connection::listener, this)) {
			if(waitable_flag::ok != _listener_thread_ready.wait(500ms)) {
				close();
				throw std::runtime_error("thread didn't start in time");
			}
		}

	stream_connection(const std::string& unix_domain_socket,
					  client_disconnect_callback_t disconnection_callback = nullptr,
					  new_data_callback_t new_data_callback = nullptr,
					  void* user_param = nullptr)
		: stream_connection(
			create_and_connect_socket(unix_domain_socket),
			std::move(disconnection_callback),
			std::move(new_data_callback),
			user_param) {
		start();
	}

	// a client should not be copied.
	stream_connection(const stream_connection&) = delete;

	~stream_connection() {
		close();

		if(_listener_thread.joinable()) {
			_listener_thread.join();
		}
	}

	waitable_condition& disconnected_signal() {
		return _disconnected;
	}

	void start() {
		_start_reading.set();
	}

	void close() {

		shutdown(_socket, SHUT_RDWR);
		::close(_socket);

		_shutdown_requested = true;

		// interrupt the thread if it is waiting to be started
		_start_reading.interrupt();
	}

	bool send(uint8_t byte) const {
		ssize_t size = ::send(_socket, &byte, 1, 0);
		return size == 1;
	}

	ssize_t send(const uint8_t* buffer, size_t size) const {
		return ::send(_socket, buffer, size, 0);
	}

private:
	static int create_and_connect_socket(const std::string& unix_domain_socket) {

		int socket = ::socket(AF_UNIX, SOCK_STREAM, 0);
		if(socket <= 0) {
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

		// Let the main thread know that this thread has started.
		_listener_thread_ready.set();

		// Wait for the main thread to trigger the start of read operations
		_start_reading.wait();

		while(!_shutdown_requested) {

			memset(buffer, 0, len);
			ssize_t received = recv(_socket, buffer, len, 0);

			if(received <= 0) {
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

		if(_disconnection_callback) {
			_disconnection_callback(*this);
		}

		_disconnected.set();
	}
};

}
