/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_LOGVIEW_HPP
#define MICROMACHINE_EMU_LOGVIEW_HPP

#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/text_display.hpp>
#include "widgets/Helpers.hpp"

class LogView : public cppurses::layout::Vertical  {
private:
	std::deque<std::string> _lines;

public:
	cppurses::Text_display& _text{this->make_child<cppurses::Text_display>()};

	bool paint_event() override {
		render();
		return Widget::paint_event();
	}

	void append_line(const std::string& txt) {
		_lines.push_back(txt + "\n");
		update();
	}

	void render() {
		_text.clear();
		size_t displayable_lines = _text.height();
		auto start = std::prev(_lines.end(), std::min(displayable_lines, _lines.size()));
		while(start != _lines.end()) {
			_text.append(*start);
			start++;
		}
	}

	bool focus_in_event() override {
		helpers::set_foreground(border, cppurses::Color::Yellow);
		this->update();
		return Widget::focus_in_event();
	}

	bool focus_out_event() override {
		helpers::set_foreground(border, cppurses::Color::White);
		this->update();
		return Widget::focus_out_event();
	}
};

#endif //MICROMACHINE_EMU_LOGVIEW_HPP
