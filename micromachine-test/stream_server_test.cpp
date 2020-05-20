//
// Created by joel on 01/05/2020.
//

#include "framework/IodeviceHelpers.hpp"
#include "io/stream_server.hpp"
#include "io/stream_connection.hpp"
#include "peripherals/iodev.hpp"
#include "utils/blocking_queue.hpp"
#include "utils/waitable_counter.hpp"
#include <chrono>
#include <fstream>
#include <gtest/gtest.h>

namespace {
	template <typename T, typename F, typename _Rep, typename _Period>
	bool value_changed_to(T expected_value, F func, const std::chrono::duration<_Rep, _Period>& timeout) {
		auto end = std::chrono::steady_clock::now() + timeout;
		while(std::chrono::steady_clock::now() < end) {
			if(func() == expected_value) {
				return true;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		return false;
	}
}

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
	static constexpr size_t n_clients = 30;

	stream_server server(dev, "dev0", "/tmp/micromachine");

	EXPECT_EQ(0, server.client_count());

	std::list<stream_connection> clients;
	for(size_t i = 0; i < n_clients; i++) {
		clients.emplace_back(server.pathname());
	}

	EXPECT_TRUE(value_changed_to(n_clients, [&]() { return server.client_count(); }, 500ms));

	for(auto& client : clients) {
		client.close();
	}

	EXPECT_TRUE(value_changed_to(0U, [&]() { return server.client_count(); }, 500ms));

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

TEST(iodeviceServer, ASingleClientReceivesTheDataItSends) {
	using namespace micromachine::system;
	using namespace std::chrono_literals;

	waitable_condition data_recevied_signal;

	echoer_iodevice<uint8_t, 1024> dev;
	stream_server server(dev, "dev0", "/tmp/micromachine");

	std::string str = "a dummy payload\n";
	std::vector<uint8_t> payload(str.begin(), str.end());

	std::vector<uint8_t> received_data;
	auto new_data_callback = [&received_data, &payload, &data_recevied_signal](const uint8_t* buffer, size_t size, void* user_data) {
		received_data.insert(received_data.end(), buffer, buffer + size);

		if(received_data.size() == payload.size()) {

			int* data_received = (int*)user_data;
			*data_received = true;
			data_recevied_signal.set();
		}
	};

	int data_received = false;
	stream_connection client(server.pathname(), nullptr, new_data_callback, &data_received);

	// make sure the client is connected before sending data.
	EXPECT_TRUE(value_changed_to(1U, [&]() { return server.client_count(); }, 500ms));

	ssize_t transmitted = client.send(payload.data(), payload.size());
	ASSERT_GT(transmitted, 0);

	EXPECT_FALSE(data_recevied_signal.wait(2000ms));
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

	std::vector<std::thread> clients_threads;

	struct parameters {
		const size_t _expected_payload_size;
		std::vector<uint8_t> received_data;
		waitable_condition all_data_has_been_received;

		parameters(size_t expected_payload_size)
			: _expected_payload_size(expected_payload_size) {}

		void append_data(stream_connection* client, const uint8_t* buffer, size_t size) {
			received_data.insert(received_data.end(), buffer, buffer + size);
			if(received_data.size() == _expected_payload_size) {
				all_data_has_been_received.set();
			}
		}
	};

	waitable_condition start_waiting_for_data;
	waitable_counter num_clients_ready(0);

	for(size_t i = 0; i < number_of_clients; ++i) {
		clients_threads.emplace_back([&expected_payload, &server, &num_clients_ready, &start_waiting_for_data]() {
			parameters params(expected_payload.size());
			stream_connection connection(server.pathname(),
										 nullptr,
										 std::bind(&parameters::append_data,
												   &params,
												   &connection,
												   std::placeholders::_1,
												   std::placeholders::_2));

			num_clients_ready.increment();
			start_waiting_for_data.wait();
			EXPECT_EQ(waitable_flag::ok,  params.all_data_has_been_received.wait(2000ms));
			EXPECT_EQ(params.received_data, expected_payload);
			connection.close();
		});
	}

	parameters params(expected_payload.size());
	stream_connection sender(server.pathname(),
							nullptr,
							std::bind(&parameters::append_data, &params, &sender, std::placeholders::_1, std::placeholders::_2));

	// make sure everyone is connected before sending data.
 	EXPECT_TRUE(value_changed_to(1 + number_of_clients, [&]() { return server.client_count(); }, 500ms));

	// make sure all clients are ready to receive data
	num_clients_ready.wait(number_of_clients);

	sender.send(expected_payload.data(), expected_payload.size());
	start_waiting_for_data.set();

	EXPECT_EQ(waitable_flag::ok, params.all_data_has_been_received.wait(2000ms));
	EXPECT_EQ(params.received_data, expected_payload);

	// give time to the server to broadcast the message to all clients
	std::this_thread::sleep_for(50ms);
	sender.close();

	for(auto& th : clients_threads) {
		if(th.joinable()) {
			th.join();
		}
	}

	server.close();
}

TEST(iodeviceServer, ClosingServerDisconnectAllClientsBeforeReturning) {
	using namespace micromachine::system;
	empty_iodevice dev;
	stream_server server(dev, "dev0", "/tmp/micromachine");
	stream_connection connection(server.pathname());
	server.close();
	EXPECT_EQ(0, server.client_count());
	EXPECT_EQ(waitable_flag::ok, connection.disconnected_signal().wait(500ms));
}

INSTANTIATE_TEST_CASE_P(RepeatMultipleTimes, RepeaterFixture, ::testing::Range(1, 25));

