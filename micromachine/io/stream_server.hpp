//
// Created by joel on 01/05/2020.
//

#ifndef MICROMACHINE_STREAM_SERVER_HPP
#define MICROMACHINE_STREAM_SERVER_HPP

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

#include "helpers/check.hpp"
#include "io/helpers.hpp"
#include "peripherals/iodev.hpp"
#include "io/client_manager.hpp"
#include "io/stream_connection.hpp"

namespace micromachine::system {

/**
 * A Stream server is able to create a unix socket domain (AF_UNIX) server on a given location.
 * The stream server will accept connection and manage a client list.
 * Typically a server could be create to expose iodevices such as usart0 at the following location
 * /tmp/micromachine/<pid>/usart0
 */
class stream_server {
private:
	std::string _device_name;
	std::string _directory;
	std::string _pathname;
	std::string _location;
	int _socket;

	iodev& _iodev;
	iopump _iopump;

	client_manager _clients;

	std::mutex _on_client_disconnect_mutex;
	std::atomic<bool> _accept_connections;

	/**
	 * Waitable signal for starting _acceptor_thread
	 */
	interruptible_signal _acceptor_thread_ready;
	std::thread _acceptor_thread;

	static constexpr int LISTEN_BACKLOG_SIZE = 5;

	static constexpr std::string_view DEFAULT_DIRECTORY = "/tmp/micromachine";

public:
	stream_server(iodev& dev, const std::string& device_name, const std::string_view& directory = DEFAULT_DIRECTORY)
		: _device_name(device_name)
		, _directory(directory)
		, _socket(create_and_bind_socket(_device_name, _directory, _pathname, _location))
		, _iodev(dev)
		, _iopump(_iodev, std::bind(&stream_server::broadcast, this, std::placeholders::_1))
		, _accept_connections(true)
		, _acceptor_thread(std::thread(&stream_server::accept_loop, this)) {

		if(interruptible_signal::ok != _acceptor_thread_ready.wait(500ms)) {
			close();
			throw std::runtime_error("thread didn't start in time");
		}
	}

	~stream_server() {
		close();
	}

	static constexpr std::string_view default_directory() {
		return DEFAULT_DIRECTORY;
	}

	const int& socket() {
		return _socket;
	}

	const std::string& pathname() const {
		return _pathname;
	}

	const std::string& location() const {
		return _location;
	}

	size_t client_size() {
		return _clients.size();
	}

	void broadcast(uint8_t byte) {
		std::lock_guard<client_manager> lock(_clients);
		for(auto& [_, client] : _clients) {
			client->send(byte);
		}
	}

	void close() {

		_iopump.wait_until_flushed();
		_iopump.shutdown();

		{
			std::lock_guard<std::mutex> lock(_on_client_disconnect_mutex);
			_accept_connections = false;
		}

		::shutdown(socket(), SHUT_RDWR);
		::close(socket());

		if(_acceptor_thread.joinable()) {
			_acceptor_thread.join();
		}

		_clients.clear();

		std::filesystem::remove_all(_pathname);

		std::error_code error;
		if(std::filesystem::is_empty(_location, error)) {
			std::filesystem::remove_all(_location);
		}
	}

private:
	/**
	 * Helper to create bind and listen to a AF_UNIX socket located in the provided directory.
	 *
	 * create a AF_UNIX socket at /<directory>/<pid>/<device_name>
	 * @param[in] device_name a device name
	 * @param[in] directory the base directory where the AF_UNIX socket will be created
	 * @param[out] pathname the full path (/tmp/micromachine/<pid>/dev0)
	 * @param[out] location AF_UNIX location directory (/tmp/micromachine/<pid>/)
	 * @return the listened socket
	 */
	static int create_and_bind_socket(const std::string& device_name,
									const std::string& directory,
									std::string& pathname,
									std::string& location) {

		int socket = ::socket(AF_UNIX, SOCK_STREAM, 0);
		if(socket == -1) {
			throw std::runtime_error("failed to create client socket: " + std::string(std::strerror(errno)));
		}

		struct sockaddr_un server;

		server.sun_family = AF_UNIX;
		location = directory + "/" + std::to_string(getpid());
		pathname = location + "/" + device_name;

		if (std::filesystem::exists(pathname)) {
			std::filesystem::remove(pathname);
		}

		if(pathname.size() > sizeof(server.sun_path)) {
			std::string message = "unix_socket_domain_path is to big! maximum allowed size is: " +
								  std::to_string(sizeof(server.sun_path));
			throw std::length_error(message);
		}

		std::filesystem::create_directories(location);

		strcpy(server.sun_path, pathname.c_str());
		int r = bind(socket, reinterpret_cast<struct sockaddr*>(&server), sizeof(server));
		if(r) {
			throw std::runtime_error("bind failed. " + std::string(std::strerror(errno)));
		}

		r = listen(socket, LISTEN_BACKLOG_SIZE);
		if(r) {
			throw std::runtime_error("listen failed. " + std::string(std::strerror(errno)));
		}

		return socket;
	}

	void data_receive_from_client_evt(const uint8_t* buffer, size_t size, void*) {
		for(size_t i = 0; i < size; ++i) {
			_iodev.send(buffer[i]);
		}
	}

	void add_new_connection(int client_socket) {
		using namespace std::placeholders;
		auto connection = std::make_unique<stream_connection>(
			client_socket,
			std::bind(&stream_server::client_disconnect_evt, this, _1),
			std::bind(&stream_server::data_receive_from_client_evt, this, _1, _2, _3),
			nullptr);

		// grab a weak reference on the connection
		// so that we can call start after moving the unique ptr
		stream_connection* client = connection.get();
		_clients.add_client(std::move(connection));
		client->start();
		_clients.delete_removed_clients();
	}

	/**
	 * Acceptor thread.
	 *   - Accept incoming connection
	 *   - Create a new stream_connection and push it to the client list
	 *   - flush the disconnected clients list
	 */
	void accept_loop() {

		// ctor synchronization
		_acceptor_thread_ready.set();

		while(_accept_connections) {

			int client_socket = ::accept(_socket, nullptr, nullptr);
			if(client_socket <= 0) {
				continue;
			}

			if(!_accept_connections) {
				break;
			}

			add_new_connection(client_socket);
		}
		_accept_connections = false;
	}

	/**
	 * Called by the client's thread
	 * @param client
	 */
	void client_disconnect_evt(stream_connection& client) {
		_clients.remove_client(client);
	}
};

} // namespace micromachine::system
#endif // MICROMACHINE_STREAM_SERVER_HPP
