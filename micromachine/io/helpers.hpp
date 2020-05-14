
//
// Created by fla on 14.05.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once

namespace micromachine::system {

/**
 * Wait on signal
 * @param sync_point
 * @param timeout_ms
 * @return true if timeout, false if the signal is catched
 */
bool wait_signal(std::promise<void>& sync_point, unsigned int timeout_ms) {
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

}
