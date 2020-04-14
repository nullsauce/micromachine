/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_LOGVIEW_HPP
#define MICROMACHINE_EMU_LOGVIEW_HPP

#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/text_display.hpp>

#include "widgets/FoldableWidgetHeader.hpp"
#include "widgets/Helpers.hpp"

#include <deque>

class LogView : public cppurses::layout::Vertical  {
private:
	FoldableWidgetHeader& _header;
	cppurses::Text_display& _text;

public:
	LogView()
		: _header(make_child<FoldableWidgetHeader>("Output"))
		, _text(make_child<cppurses::Text_display>()) {

		_text.disable_word_wrap();
	}

	void append_char(char c) {
		_text.append(c);

		if(_text.display_height() == _text.height()) {
			// text is full
			_text.scroll_down();
		}
		update();
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
