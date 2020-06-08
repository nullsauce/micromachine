
#include "ValueFormaters.hpp"

namespace micromachine::testing {

	std::string uint8ToStr(uint8_t byte) {
		char buf[32] = {0};
		snprintf(buf, sizeof(buf), "0x%02x (%d)", byte, byte);
		return std::string(buf);
	}

	std::string uint32ToStr(uint32_t value) {
		char buf[32] = {0};
		snprintf(buf, sizeof(buf), "0x%08x (%d)", value, value);
		return std::string(buf);
	}

	std::string uint16ToStr(uint16_t value) {
		char buf[32] = {0};
		snprintf(buf, sizeof(buf), "0x%04x (%d)", value, value);
		return std::string(buf);
	}

	std::string registerName(micromachine::system::reg_idx regIdx) {
		static const char* names[] = {
			"R0",
			"R1",
			"R2",
			"R3",
			"R4",
			"R5",
			"R6",
			"R7",
			"R8",
			"R9",
			"R10",
			"R11",
			"R12",
			"SP (R13)",
			"LR (R14)",
			"PC (R15)",
		};
		return names[regIdx & 0xf];
	}

}
