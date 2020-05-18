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
	EXPECT_EQ(std::string(stream_server::DEFAULT_DIRECTORY) + "/" + std::to_string(getpid()) + "/dev0",
			  server.pathname());
	server.close();
	EXPECT_FALSE(std::filesystem::exists(server.pathname()));
}

TEST(streamServer, InstanciateServerWithValidUnixDomainSocket) {
	using namespace micromachine::system;
	empty_iodevice dev;

	std::filesystem::path directory = "/tmp/micromachine";
	stream_server server(dev, "dev0", directory);
	EXPECT_EQ((directory / std::to_string(getpid()) / "dev0").string(), server.pathname());
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
	std::filesystem::create_directories(location);
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

	while(n_clients != server.client_count()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

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

TEST(iodeviceServer, ASignleClientReceivesTheDataItSends) {
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

	// make sure the client is connected before sending data.
	while(server.client_count() != 1) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	ssize_t transmitted = client.send(payload.data(), payload.size());
	ASSERT_GT(transmitted, 0);

	EXPECT_FALSE(signal.wait(1000));
	EXPECT_TRUE(data_received);
	EXPECT_EQ(payload, received_data);

	client.close();
	server.close();
}

TEST_P(RepeaterFixture, DataSentByOneClientIsBroadcastToAllClientsIncludingItself) {
	using namespace micromachine::system;

	const std::string str = "A funny payload for multiple clients";
	const std::vector<uint8_t> expected_payload(str.begin(), str.end());
	const size_t number_of_clients = 20;

	echoer_iodevice<uint8_t, 1024> echo_device;
	stream_server server(echo_device, "dev0", "/tmp/micromachine");

	std::list<std::thread> clients_threads(number_of_clients);

	struct parameters {
		const size_t _expected_payload_size;
		std::vector<uint8_t> received_data;
		waitable_condition all_data_has_been_received;

		parameters(size_t expected_payload_size)
		 : _expected_payload_size(expected_payload_size) {}

		void append_data(const uint8_t* buffer, size_t size) {
			received_data.insert(received_data.end(), buffer, buffer + size);
			if(received_data.size() == _expected_payload_size) {
				all_data_has_been_received.set();
			}
		}
	};

	waitable_condition start_waiting_for_data;

	for(auto& th : clients_threads) {
		th = std::thread([&server, &expected_payload, &start_waiting_for_data]() {
			parameters params(expected_payload.size());
			stream_connection connection(server.pathname(), nullptr, std::bind(&parameters::append_data, &params, std::placeholders::_1, std::placeholders::_2));
			start_waiting_for_data.wait();
			EXPECT_EQ(waitable_flag::ok, params.all_data_has_been_received.wait(1000ms));
			EXPECT_EQ(params.received_data, expected_payload);
			connection.close();
		});
	}

	// make sure everyone is connected before sending data.
	while(server.client_count() != number_of_clients) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	parameters params(expected_payload.size());
	stream_connection sender(server.pathname(), nullptr,
							 std::bind(&parameters::append_data, &params, std::placeholders::_1, std::placeholders::_2));

	start_waiting_for_data.set();
	sender.send(expected_payload.data(), expected_payload.size());

	EXPECT_EQ(waitable_flag::ok, params.all_data_has_been_received.wait(1000ms));
	EXPECT_EQ(params.received_data, expected_payload);

	// give time to the server to broadcast the message to all clients
	std::this_thread::sleep_for(100ms);
	sender.close();

	for(auto& th : clients_threads) {
		if(th.joinable()) {
			th.join();
		}
	}

	server.close();
}

INSTANTIATE_TEST_CASE_P(RepeatMultipleTimes, RepeaterFixture, ::testing::Range(1, 25));

