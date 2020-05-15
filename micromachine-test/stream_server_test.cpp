//
// Created by joel on 01/05/2020.
//

#include "framework/IodeviceHelpers.hpp"
#include "io/stream_server.hpp"
#include "io/stream_connection.hpp"
#include "peripherals/iodev.hpp"
#include "utils/blocking_queue.hpp"
#include <chrono>
#include <fstream>
#include <gtest/gtest.h>

class waitable_signal {
private:
	std::promise<void> sync_point;

public:
	/**
	 *
	 * @param timeout_ms
	 * @return true if timeout, false is the signal has been notified before timeout_ms
	 */
	bool wait(unsigned int timeout_ms = 0) {

		auto f = sync_point.get_future();

		try {
			if(timeout_ms) {

				std::future_status status;
				do {
					status = f.wait_for(std::chrono::milliseconds(timeout_ms));
					if(status == std::future_status::deferred) {
						continue;
					} else if(status == std::future_status::timeout) {
						return true;
					}
				} while(status != std::future_status::ready);
			} else {
				f.wait();
			}
		} catch(std::future_error& e) {
			if(e.code() == std::future_errc::no_state) {
				return false;
			}
		}
		return false;
	}

	void notify() {
		sync_point.set_value();
	}
};

class RepeaterFixture : public ::testing::TestWithParam<int> {};

TEST_P(RepeaterFixture, createOne_stream_server) {
	using namespace micromachine::system;
	empty_iodevice dev;
	stream_server server(dev, "dev0", "/tmp/micromachine");
}

TEST(streamServer, UnixSocketDomainCreationAndSuppression) {
	using namespace micromachine::system;
	empty_iodevice dev;

	stream_server server(dev, "dev0", "/tmp/micromachine");
	EXPECT_TRUE(std::filesystem::exists(server.pathname()));
	server.close();
	EXPECT_FALSE(std::filesystem::exists(server.pathname()));
}

TEST(streamServer, UnixSocketDomainCreationAndSuppressionWithDefaultDirectory) {
	using namespace micromachine::system;
	empty_iodevice dev;

	stream_server server(dev, "dev0");

	EXPECT_TRUE(std::filesystem::exists(server.pathname()));
	EXPECT_EQ(std::string(stream_server::default_directory()) + "/" + std::to_string(getpid()) + "/dev0",
			  server.pathname());
	server.close();
	EXPECT_FALSE(std::filesystem::exists(server.pathname()));
}

TEST(streamServer, InstanciateServerWithValidUnixDomainSocket) {
	using namespace micromachine::system;
	empty_iodevice dev;

	std::string unix_domain_socket = "/tmp/micromachine";
	stream_server server(dev, "dev0", unix_domain_socket);
	EXPECT_EQ((unix_domain_socket + "/" + std::to_string(getpid())), server.location());
}

TEST(streamServer, InstanciateServerWithInvalidUnixDomainSocketMustThrow_length_error) {
	using namespace micromachine::system;
	empty_iodevice dev;

	static const std::string a_very_long_path =
		"/tmp/micromachine/AAAA/AAAA/AAAA/AAAA/AAAA/AAAA/AAAA/AAAA/AAAA/AAAA/AAAA/AAAA/AAAA/AAAA/AAAA/AAAA/AAAA/AAAA";

	EXPECT_THROW(stream_server(dev, "dev0", a_very_long_path), std::length_error);
}

TEST(streamServer, InstanciateServerWithExistingFileMustPass) {
	using namespace micromachine::system;
	empty_iodevice dev;

	static const std::string location = "/tmp/micromachine/" + std::to_string(getpid());
	static const std::string device_name = "dev0";
	static const std::string file_path = location + '/' + device_name;
	EXPECT_TRUE(std::filesystem::create_directories(location));
	std::cerr << file_path << std::endl;
	std::ofstream file(file_path);
	EXPECT_NO_THROW(stream_server(dev, device_name, "/tmp/micromachine"));
}

TEST(streamServer, ClientSizeMustBeZeroAfterServerInstanciation) {
	using namespace micromachine::system;
	empty_iodevice dev;

	stream_server server(dev, "dev0", "/tmp/micromachine");
	EXPECT_EQ(0, server.client_count());
	server.close();
}

TEST_P(RepeaterFixture, createOne_stream_connection) {
	using namespace micromachine::system;
	empty_iodevice dev;

	stream_server server(dev, "dev0", "/tmp/micromachine");
	stream_connection connection(server.pathname());
}

TEST(streamConnection, ClientConnectionDataCoherance) {
	using namespace micromachine::system;
	empty_iodevice dev;

	stream_server server(dev, "dev0", "/tmp/micromachine");

	stream_connection client(server.pathname());
	EXPECT_GT(client.socket(), 0);
	client.close();
	server.close();
}

TEST(streamConnection, ConnectMustFail) {
	using namespace micromachine::system;
	EXPECT_THROW(stream_connection connection("/tmp/invalid/path/name"), std::runtime_error);
}

TEST(streamConnection, ConnectMustSucceed) {
	using namespace micromachine::system;
	empty_iodevice dev;

	stream_server server(dev, "dev0", "/tmp/micromachine");
	stream_connection connection(server.pathname());
}

TEST_P(RepeaterFixture, ConnectDisconnectSeveralClientAndCheckServerCoherance) {
	using namespace micromachine::system;
	empty_iodevice dev;

	// will create n threads
	static constexpr unsigned int n_clients = 30;

	stream_server server(dev, "dev0", "/tmp/micromachine");

	EXPECT_EQ(0, server.client_count());

	std::list<stream_connection> clients;
	for(unsigned int i = 0; i < n_clients; i++) {
		clients.emplace_back(server.pathname());
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	EXPECT_EQ(n_clients, server.client_size());

	for(auto& client : clients) {
		client.close();
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	EXPECT_EQ(0, server.client_count());
	server.close();
}

TEST(iodeviceServer, createOne) {
	using namespace micromachine::system;

	empty_iodevice dev;
	stream_server server(dev, "dev0", "/tmp/micromachine");
}

TEST(iodeviceServer, createAndStop) {
	using namespace micromachine::system;

	empty_iodevice dev;
	stream_server server(dev, "dev0", "/tmp/micromachine");
	server.close();
}

TEST(iodeviceServer, EchoWithOneClient) {
	using namespace micromachine::system;
	using namespace std::chrono_literals;

	waitable_signal signal;

	echoer_iodevice<uint8_t, 1024> dev;
	stream_server server(dev, "dev0", "/tmp/micromachine");

	std::string str = "a dummy payload\n";
	std::vector<uint8_t> payload(str.begin(), str.end());

	std::vector<uint8_t> received_data;
	auto new_data_callback = [&received_data, &payload, &signal](const uint8_t* buffer, size_t size, void* user_data) {
		received_data.insert(received_data.end(), buffer, buffer + size);

		if(received_data.size() == payload.size()) {

			int* data_received = (int*)user_data;
			*data_received = true;
			signal.notify();
		}
	};

	int data_received = false;
	stream_connection client(server.pathname(), nullptr, new_data_callback, &data_received);

	ssize_t transmitted = client.send(payload.data(), payload.size());
	ASSERT_GT(transmitted, 0);

	EXPECT_FALSE(signal.wait(1000));
	EXPECT_TRUE(data_received);
	EXPECT_EQ(payload, received_data);

	client.close();
	server.close();
}

TEST_P(RepeaterFixture, EchoWithSeveralClients) {
	using namespace micromachine::system;

	echoer_iodevice<uint8_t, 1024> dev;
	stream_server server(dev, "dev0", "/tmp/micromachine");
	std::string str = "A funny payload for multiple clients";
	std::vector<uint8_t> payload(str.begin(), str.end());

	size_t n_clients = 20;
	std::list<std::thread> clients_thr(n_clients);

	struct parameters {
		std::vector<uint8_t> received_data;
		waitable_signal all_data_has_been_received;
	};
	auto new_data_callback = [&payload](const uint8_t* buffer, size_t size, void* user_param) {
		parameters* params = static_cast<parameters*>(user_param);
		params->received_data.insert(params->received_data.end(), buffer, buffer + size);

		if(params->received_data == payload) {
			params->all_data_has_been_received.notify();
		}
	};

	std::atomic<size_t> count = 0;
	for(auto& th : clients_thr) {
		auto* param = new parameters;
		th = std::thread([&server, &new_data_callback, param, &payload, &count]() {
			stream_connection connection(server.pathname(), nullptr, new_data_callback, param);
			count++;
			EXPECT_FALSE(param->all_data_has_been_received.wait());
			EXPECT_EQ(param->received_data, payload);
			connection.close();
			delete param;
		});
	}

	// make sure everyone is listening.
	while(count != n_clients);

	parameters param;
	stream_connection sender(server.pathname(), nullptr, new_data_callback, &param);
	sender.send(payload.data(), payload.size());

	// make sure the message is broadcasted by the server
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	sender.close();

	for(auto& th : clients_thr) {
		if(th.joinable()) {
			th.join();
		}
	}

	server.close();
}

INSTANTIATE_TEST_CASE_P(RepeatMultipleTimes, RepeaterFixture, ::testing::Range(1, 25));

