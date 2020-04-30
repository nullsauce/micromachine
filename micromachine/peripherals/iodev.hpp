
//
// Created by fla on 30.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

#include <thread>

class iodev {
public:
	virtual void send(uint8_t byte) = 0;
	virtual bool receive(uint8_t& byte) = 0;
	virtual void shutdown() = 0;
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
		, _thread(&iopump::loop, this)
	{}

	void shutdown() {
		_aborted = true;
		_iodev.shutdown();
		_thread.join();
	}

private:
	void loop() {
		while (!_aborted) {
			uint8_t byte;
			if(_iodev.receive(byte)) {
				_rx_callback(byte);
			}
		}
	}

};
