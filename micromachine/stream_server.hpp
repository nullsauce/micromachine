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
#include "peripherals/iodev.hpp"

namespace micromachine::system {

/**
 * Wait on signal
 * @param sync_point
 * @param timeout_ms
 * @return true if timeout, false if the signal is catched
 */
static bool wait_signal(std::promise<void>& sync_point, unsigned int timeout_ms) {
	using namespace std::chrono_literals;

	auto f = sync_point.get_future();

	std::future_status status;
	do {
		status = f.wait_for(std::chrono::milliseconds(timeout_ms));
		if(status == std::future_status::deferred) {
			continue;
		} else if(status == std::future_status::timeout) {
			return true;
		}
	} while(status != std::future_status::ready);

	return false;
}

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

	bool _is_connected;

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

		// since create_and_connect_socket hasn't throw, assume it's connected
		_is_connected = true;

		wait_signal_or_throw(_listener_has_stared, _listener_thread);
	}

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
		, _is_connected(true)
		, _listener_is_running(true)
		, _listener_thread(std::thread(&stream_connection::listener, this)) {

		wait_signal_or_throw(_listener_has_stared, _listener_thread);
	}

	~stream_connection() {
		close();
	}

	bool is_connected() const {
		return _is_connected;
	}

	const int& socket() const {
		return _socket;
	}

	int close() {

		if(!_is_connected) {
			return 0;
		}

		_is_connected = false;

		::shutdown(_socket, SHUT_RDWR);
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
			std::perror("send failed!");
			return;
		}
	}

	ssize_t send(const uint8_t* buffer, size_t size) const {
		return ::send(_socket, buffer, size, 0);
	}

private:
	static void wait_signal_or_throw(std::promise<void>& sync_point, std::thread& th) {
		bool timeout = wait_signal(sync_point, 500);
		if(timeout) {
			if(th.joinable()) {
				th.join();
			}
			throw std::runtime_error("signal cannot be stopped on time");
		}
	}

	static int create_and_connect_socket(const std::string& unix_domain_socket) {

		int socket = ::socket(AF_UNIX, SOCK_STREAM, 0);
		if(socket == -1) {
			throw std::runtime_error("failed to create client socket: " + std::string(std::strerror(errno)));
		}

		struct sockaddr_un server_address = {};
		server_address.sun_family = AF_UNIX;

		std::strcpy(server_address.sun_path, unix_domain_socket.c_str());

		int connect_status = ::connect(socket, (const struct sockaddr*)&server_address, sizeof(server_address));
		if(connect_status) {
			throw std::runtime_error("failed to create connect: " + std::string(std::strerror(errno)));
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
		_listener_has_stared.set_value();

		while(_listener_is_running) {
			if(!is_connected()) {
				std::this_thread::sleep_for(std::chrono::microseconds(10));
				continue;
			}

			memset(buffer, 0, len);
			ssize_t received = ::recv(_socket, buffer, len, 0);
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

/**
 * Since the client list should be accessed by multiple threads, provide a thread safe client manager.
 */
class client_manager {
private:
	using af_unix_connection_type = std::unique_ptr<stream_connection>;
	using connection_pair = std::pair<int, af_unix_connection_type>;
	std::mutex _clients_mutex;
	std::map<int, af_unix_connection_type> _clients;
	std::list<af_unix_connection_type> _clients_to_delete;

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

/**
 * A Stream server is able to create a unix socket domain (AF_UNIX) server on a given location.
 * The stream server will accept connection and manage a client list.
 * Typically a server could be create to expose iodevices such as usart0 at the following location /tmp/micromachine/<pid>/usart0
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
	std::atomic<bool> _acceptor_is_running;

	/**
	 * Waitable signal for starting _acceptor_thread
	 */
	std::promise<void> _acceptor_is_started;
	std::thread _acceptor_thread;

	static constexpr int LISTEN_BACKLOG_SIZE = 5;

	static constexpr std::string_view DEFAULT_DIRECTORY = "/tmp/micromachine";

public:

	stream_server(iodev& dev, const std::string& device_name, const std::string_view& directory = DEFAULT_DIRECTORY)
		: _device_name(device_name)
		, _directory(directory)
		, _socket(create_and_bind_socket(_device_name, _directory, _pathname, _location))
		, _iodev(dev)
		, _iopump(_iodev, std::bind(&client_manager::broadcast, &_clients, std::placeholders::_1))
		, _acceptor_is_running(true)
		, _acceptor_thread(std::thread(&stream_server::acceptor, this)) {

		bool timeout = wait_signal(_acceptor_is_started, 500);
		if(timeout) {
			_acceptor_is_running = false;
			if(_acceptor_thread.joinable()) {
				_acceptor_thread.join();
			}
			throw std::runtime_error("acceptor cannot be started on time");
		}
	}

	virtual ~stream_server() {
		stop();
	}

	static constexpr std::string_view default_directory() {
		return DEFAULT_DIRECTORY;
	}

	void stop() {
		close();
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

	int close() {
		{
			std::lock_guard<std::mutex> lock(_on_client_disconnect_mutex);
			_acceptor_is_running = false;
		}
		::shutdown(socket(), SHUT_RDWR);
		int r = ::close(socket());

		if(_acceptor_thread.joinable()) {
			_acceptor_thread.join();
		}

		_iopump.wait_until_flushed();
		_iopump.shutdown();

		_clients.clear();

		std::filesystem::remove_all(_pathname);

		std::error_code error;
		if(std::filesystem::is_empty(_location, error)) {
			std::filesystem::remove_all(_location);
		}

		return r;
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
			pathname(),
			std::bind(&stream_server::client_disconnect_evt, this, _1),
			std::bind(&stream_server::data_receive_from_client_evt, this, _1, _2, _3),
			nullptr);

		_clients.add_new_client(std::make_pair(client_socket, std::move(connection)));
		_clients.flush_delete_list();
	}

	/**
	 * Acceptor thread.
	 *   - Accept incomming connection
	 *   - Create a new stream_connection and push it to the client list
	 *   - flush the disconnected clients list
	 */
	void acceptor() {

		// ctor synchronization
		_acceptor_is_started.set_value();
		while(_acceptor_is_running) {

			int client_socket = ::accept(_socket, nullptr, nullptr);
			if(client_socket <= 0) {
				std::perror("accept failed");
				fprintf(stderr, "socket: %d\n", _socket);
				continue;
			}

			if(!_acceptor_is_running)
				break;

			add_new_connection(client_socket);
		}
		_acceptor_is_running = false;
	}

	void client_disconnect_evt(stream_connection& client) {

		/*
		 * since this event and close() member function could be called at the same time by two different thread,
		 * _acceptor_is_running should be protected.
		 */
		std::lock_guard<std::mutex> lock(_on_client_disconnect_mutex);

		// If acceptor thread is not running all client will be removed in the close() member function
		if(_acceptor_is_running) {
			_clients.remove_client(client);
		}
	}
};

namespace {
std::unique_ptr<stream_server>
create_stream_server(iodev& dev,
					 const std::string& device_name,
					 const std::string_view& directory = stream_server::default_directory()) {
	try {
		return std::make_unique<stream_server>(dev, device_name, directory);
	} catch(std::runtime_error& e) {
		fprintf(stderr, "Error: stream server cannot be started: %s\n", e.what());
	} catch(std::length_error& e) {
		fprintf(stderr, "Error: stream server cannot be started, unix domain socket path is too long\n");
	}
	return nullptr;
}
} // namespace

} // namespace micromachine::system
#endif // MICROMACHINE_STREAM_SERVER_HPP
