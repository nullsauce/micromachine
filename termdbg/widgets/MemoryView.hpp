/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/text_display.hpp>
#include <memory/mapping.hpp>
#include <sstream>

class MemoryView : public cppurses::layout::Vertical {
protected:
	uint32_t _segment_offset;
	uint32_t _cursor_x;
	uint32_t _cursor_y;
	cppurses::Text_display& _text;
	const memory_mapping* _segment;
	std::vector<char> _line_buffer;

public:
	MemoryView()
		: _segment_offset(0)
		, _cursor_x(0)
		, _cursor_y(0)
		, _text(make_child<cppurses::Text_display>())
		, _segment(nullptr) {

	}

	void set_segment(const memory_mapping* segment) {
		if(_segment != segment) {
			_segment = segment;
			navigate_to_offset(0);
			update();
		}
	}

	bool paint_event() override {
		render();
		update();
		return Widget::paint_event();
	}

	void scroll_byte_up() {
		scroll_up(1);
	}

	void scroll_byte_down() {
		scroll_down(1);
	}

	void scroll_line_up() {
		scroll_up(displayable_clomuns());
	}
	void scroll_line_down() {
		scroll_down(displayable_clomuns());
	}

	void scroll_page_up() {
		scroll_up(displayable_bytes());
	}
	void scroll_page_down() {
		scroll_down(displayable_bytes());
	}

	void scroll_up(uint32_t offset) {
		set_segment_offset(std::min(_segment_offset + offset, _segment->size() - 1));
	}

	void scroll_down(uint32_t offset) {
		set_segment_offset(std::min(_segment_offset - offset, _segment->size() - 1));
	}

	void set_segment_offset(uint32_t segment_offset) {
		_segment_offset = segment_offset;
		update();
	}

	void navigate_to_offset(uint32_t offset) {
		_cursor_x = 0;
		_cursor_y = 0;
		set_segment_offset(offset);
	}

	uint32_t address_to_segment(uint32_t address) {
		return address - _segment->start();
	}

	void navigate_to_address(uint32_t address) {
		set_segment_offset(address_to_segment(address));
	}

	void move_cursor_left() {
		if(_cursor_x == 0) {
			if(is_valid_segment_offset(segment_offset_at_cursor_pos(displayable_clomuns()-1, _cursor_y))) {
				_cursor_x = displayable_clomuns()-1;
			}
		} else {
			if(is_valid_segment_offset(segment_offset_at_cursor_pos(_cursor_x - 1, _cursor_y))) {
				_cursor_x--;
			}
		}
		update();
	}

	void move_cursor_right() {
		if(_cursor_x == displayable_clomuns() - 1) {
			if(is_valid_segment_offset(segment_offset_at_cursor_pos(0, _cursor_y))) {
				_cursor_x = 0;
			}
		} else {
			if(is_valid_segment_offset(segment_offset_at_cursor_pos(_cursor_x + 1, _cursor_y))) {
				_cursor_x++;
			}
		}
		update();
	}

	void move_cursor_up() {
		if(is_valid_segment_offset(segment_offset_at_cursor_pos(_cursor_x, _cursor_y - 1))) {
			if(_cursor_y == 0) {
				scroll_line_down();
			} else {
				_cursor_y--;
			}
		}
		update();
	}

	void move_cursor_down() {
		if(is_valid_segment_offset(segment_offset_at_cursor_pos(_cursor_x, _cursor_y + 1))) {
			if(_cursor_y < displayable_lines() - 1) {
				_cursor_y++;
			} else {
				scroll_line_up();
			}
		}
		update();
	}

	uint32_t segment_offset_at_cursor_pos(uint32_t cursor_x, uint32_t cursor_y) const {
		return _segment_offset + (cursor_y * displayable_clomuns()) + (cursor_x);
	}

	uint32_t cursor_offset() const {
		return segment_offset_at_cursor_pos(_cursor_x, _cursor_y);
	}

	size_t displayable_lines() const {
		return _text.height();
	}

	uint32_t displayed_bytes() {
		return std::min(_segment_offset + displayable_bytes(), _segment->size());
	}

	size_t max_displayable_clomuns() const {
		return std::max(0,((int)_text.width()-10)) / 3;
	}

	size_t displayable_clomuns() const {
		return (max_displayable_clomuns() / 4) * 4;
	}

	uint32_t displayable_bytes() const {
		return displayable_clomuns() * displayable_lines();
	}

	bool is_valid_segment_offset(uint32_t segment_offset) {
		return segment_offset < _segment->size();
	}

	void render() {
		_text.clear();
		if(nullptr == _segment) return;
		const uint8_t* segment_base = _segment->host_mem();

		_line_buffer.reserve(16 + (displayable_clomuns() * 3));
		std::stringstream ss;
		for(size_t l = 0; l < displayable_lines(); l++) {
			const uint32_t line_offset = _segment_offset + (l * displayable_clomuns());
			if(!is_valid_segment_offset(line_offset)) {
				break;
			}

			std::sprintf(_line_buffer.data(), "%08x│", (uint32_t)line_offset + _segment->start());
			_text.append(_line_buffer.data());
			for(size_t c = 0; c < displayable_clomuns(); c++) {
				uint32_t byte_offset = line_offset + c;
				if(!is_valid_segment_offset(byte_offset)) {
					break;
				}
				std::sprintf(_line_buffer.data(), "%02x ", segment_base[byte_offset]);
				if(cursor_offset() == byte_offset) {
					_text.insert_brush.add_attributes(cppurses::Attribute::Standout);
					_text.append(_line_buffer.data());
					_text.insert_brush.remove_attributes(cppurses::Attribute::Standout);
				} else {
					_text.append(_line_buffer.data());
				}

			}
		}
		_text.append("\n");
	}
};

