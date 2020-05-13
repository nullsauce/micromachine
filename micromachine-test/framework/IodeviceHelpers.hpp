//
// Created by joel on 13/05/2020.
//

#ifndef MICROMACHINE_IODEVICEHELPERS_HPP
#define MICROMACHINE_IODEVICEHELPERS_HPP

#include "peripherals/iodev.hpp"

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


class empty_iodevice : public iodev {
public:
	void send(uint8_t byte) override {}
	bool receive(uint8_t& byte) override {
		return false;
	}
	void shutdown() override {}

	size_t bytes_available() const override {
		return 0;
	}
};

#endif // MICROMACHINE_IODEVICEHELPERS_HPP
