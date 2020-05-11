
//
// Created by fla on 30.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include "utils/blocking_queue.hpp"
#include <functional>
#include <string>
#include <thread>

class iodev {
public:
	virtual void send(uint8_t byte) = 0;
	virtual bool receive(uint8_t& byte) = 0;
	/**
	 * The device has at least that many bytes available for receiving.
	 * @return
	 */
	virtual size_t  bytes_available() const = 0;
	virtual void shutdown() = 0;
	void write_string(const std::string& string) {
		for(char c : string) {
			send(c);
		}
	}
};

/**
 * An echoer iodevice that act as a loop back.
 *
 * There is no rx buffer. send push into tx buffer, and receive pop it.
 *
 * @tparam buffer_type
 * @tparam buffer_size
 */
template <typename buffer_type, size_t buffer_size>
class echoer_iodevice : public iodev {
private:
	using data_channel = blocking_bounded_channel<buffer_type, buffer_size>;
	data_channel _tx_buffer;

public:
	/**
	 * Send byte to the tx buffer and create a loopback for echoing messages
	 * @param byte
	 */
	void send(uint8_t byte) override {
		_tx_buffer.force_push(byte);
	}

	bool receive(uint8_t& out) override {
		return data_channel::success == _tx_buffer.pop(out);
	}

	void shutdown() override {
		_tx_buffer.abort();
	}

	size_t bytes_available() const override {
		return _tx_buffer.size();
	}
};

class iopump {
public:
	using rx_callback = std::function<void(uint8_t byte)>;

private:
	iodev& _iodev;
	rx_callback _rx_callback;
	bool _aborted;
	std::thread _thread;

public:
	iopump(iodev& iodev, rx_callback rx_callback)
		: _iodev(iodev)
		, _rx_callback(rx_callback)
		, _aborted(false)
		, _thread(&iopump::loop, this) {}

	/**
	 * Waits at most timeout_millis milliseconds for all the underlying device's data to be passed to the callback.
	 * @param timeout_millis The maximmum amount of time this operation is given. Pass zero for infinite.
	 * @return true if all the data could be flushed before the timeout
	 */
	bool wait_until_flushed(uint64_t timeout_millis = 0) {
		auto start = std::chrono::steady_clock::now();
		while(_iodev.bytes_available() > 0) {
			if(timeout_millis) {
				auto elapsed = std::chrono::steady_clock::now() - start;
				uint64_t elapsed_millis = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed).count();
				if(elapsed_millis > timeout_millis) {
					return _iodev.bytes_available() == 0;
				}
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		return true;
	}

	void shutdown() {
		_aborted = true;
		_iodev.shutdown();
		if(_thread.joinable()) {
			_thread.join();
		}
	}

private:
	void loop() {
		while(!_aborted) {
			uint8_t byte;
			if(_iodev.receive(byte)) {
				_rx_callback(byte);
			}
		}
	}
};
