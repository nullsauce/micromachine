/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_INFOTABLE_HPP
#define MICROMACHINE_EMU_INFOTABLE_HPP

#include <memory/mapping.hpp>

#include "mapping.hpp"
#include "widgets/RightAlignedText.hpp"
#include "widgets/Table.hpp"

class InfoTable : public Table {
private:
	const memory_mapping* _segment = nullptr;
	uint32_t _segment_offset = 0;

public:
	using Table::Table;

	void set_segment(const memory_mapping* segment) {
		if(_segment != segment) {
			_segment = segment;
			update();
		}
	}

	void set_segment_offset(uint32_t offset) {
		if(offset != _segment_offset) {
			_segment_offset = offset;
			update();
		}
	}

private:
	bool paint_event() override {
		render();
		return Widget::paint_event();
	}

	void render() {

		if(nullptr == _segment) return;

		const uint8_t* ptr = _segment->host_mem() + _segment_offset;

		uint8_t u8le = *(uint8_t*)ptr;
		uint16_t u16le = *(uint16_t*)ptr;
		uint32_t u32le = *(uint32_t*)ptr;
		int8_t i8le = *(int8_t*)ptr;
		int16_t i16le = *(int16_t*)ptr;
		int32_t i32le = *(int32_t*)ptr;
		uint8_t u8be = u8le;
		uint16_t u16be = __builtin_bswap16(u16le);
		uint32_t u32be = __builtin_bswap32(u32le);
		int8_t i8be = i8le;
		int16_t i16be = __builtin_bswap16(i16le);
		int32_t i32be =__builtin_bswap16(i32le);

		get_cell_at<RightAlignedText>(1,1).set_contents(std::to_string(u8le));
		get_cell_at<RightAlignedText>(2,1).set_contents(std::to_string(u16le));
		get_cell_at<RightAlignedText>(3,1).set_contents(std::to_string(u32le));
		get_cell_at<RightAlignedText>(1,2).set_contents(std::to_string(i8le));
		get_cell_at<RightAlignedText>(2,2).set_contents(std::to_string(i16le));
		get_cell_at<RightAlignedText>(3,2).set_contents(std::to_string(i32le));

		get_cell_at<RightAlignedText>(1,3).set_contents(std::to_string(u8be));
		get_cell_at<RightAlignedText>(2,3).set_contents(std::to_string(u16be));
		get_cell_at<RightAlignedText>(3,3).set_contents(std::to_string(u32be));
		get_cell_at<RightAlignedText>(1,4).set_contents(std::to_string(i8be));
		get_cell_at<RightAlignedText>(2,4).set_contents(std::to_string(i16be));
		get_cell_at<RightAlignedText>(3,4).set_contents(std::to_string(i32be));
	}
};

#endif //MICROMACHINE_EMU_INFOTABLE_HPP
