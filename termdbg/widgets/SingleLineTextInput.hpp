/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include <cppurses/widget/widgets/textbox.hpp>

#include "widgets/Helpers.hpp"

class SingleLineTextInput : public cppurses::Textbox {
public:
	bool _placeholder_is_set;
	sig::Signal<void(const cppurses::Glyph_string&)> text_changed;
	sig::Signal<void(const cppurses::Glyph_string&)> text_entered;
	sig::Signal<void()> input_aborted;
	cppurses::Glyph_string _placeholder_text;

	SingleLineTextInput(const cppurses::Glyph_string& placeholder)
		: _placeholder_is_set(true)
		, _placeholder_text(placeholder) {
		enable_placeholder();
		height_policy.fixed(3);
		border.enable();
		disable_word_wrap();
	}

	bool key_press_event(const cppurses::Key::State& keyboard) override {
		if(keyboard.key == cppurses::Key::Enter) {
			text_entered(this->contents());
			return false;
		} else if(keyboard.key == cppurses::Key::Escape) {
			input_aborted();
			return false;
		} else if(contents().empty()) {
			enable_placeholder();
			return false;
		} else {
			if(_placeholder_is_set) {
				disable_placeholder();
			}

			bool res = Textbox::key_press_event(keyboard);
			text_changed(this->contents());
			return res;
		}
	}

	void enable_placeholder() {
		brush.set_foreground(cppurses::Color::Dark_gray);
		set_contents(_placeholder_text);
		_placeholder_is_set = true;
	}

	void disable_placeholder() {
		brush.set_foreground(cppurses::Color::White);
		clear();
		_placeholder_is_set = false;
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

