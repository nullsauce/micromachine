
//
// Created by fla on 27.04.20.
// Copyright (c) 2020 The Micromachine Project. All rights reserved.
//

#pragma once


namespace micromachine::testing {

	static std::string uint8ToStr(uint8_t byte) {
		char buf[32] = {0};
		snprintf(buf, sizeof(buf), "0x%02x (%d)", byte, byte);
		return std::string(buf);
	}

	static std::string uint32ToStr(uint32_t value) {
		char buf[32] = {0};
		snprintf(buf, sizeof(buf), "0x%08x (%d)", value, value);
		return std::string(buf);
	}

	static std::string uint16ToStr(uint16_t value) {
		char buf[32] = {0};
		snprintf(buf, sizeof(buf), "0x%04x (%d)", value, value);
		return std::string(buf);
	}

}
