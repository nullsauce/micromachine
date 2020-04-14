/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_ADDRESSINPUT_HPP
#define MICROMACHINE_EMU_ADDRESSINPUT_HPP


#include "widgets/SingleLineTextInput.hpp"
#include "widgets/Helpers.hpp"

class AddressInput : public SingleLineTextInput {
private:
	uint32_t _address;
	const int _max_chars;
	int _min_chars;
	bool _parse_success;
	bool _notification_visible;
	cppurses::Glyph_string _original_content;

public:
	sig::Signal<void()> validated;

	AddressInput(cppurses::Glyph_string placeholder = "address")
		: SingleLineTextInput(std::move(placeholder))
		, _address(0)
		, _max_chars(8)
		, _min_chars(1)
		, _parse_success(false)
		, _notification_visible(false) {
		text_changed.connect(std::bind(&AddressInput::try_parse, this, std::placeholders::_1));
		text_entered.connect(std::bind(&AddressInput::validate, this, std::placeholders::_1));
	}

	void set_min_chars(int value) {
		_min_chars = value;
	}

	uint32_t address() const {
		return _address;
	}

	bool parse_success() const {
		return _parse_success;
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

	void mark_invalid() {
		helpers::set_foreground(border, cppurses::Color::Yellow);
		show_noitification("invalid address");
	}

	bool key_press_event(const cppurses::Key::State& keyboard) override {
		hide_notification();
		return SingleLineTextInput::key_press_event(keyboard);
	}

private:

	void show_noitification(cppurses::Glyph_string notification) {
		hide_notification();
		_original_content = contents();
		set_contents(FormatedText()
			.write(_original_content)
			.write(" ")
			.write_color_f(std::move(notification), cppurses::Color::Red)
			.str()
		);
		set_cursor({_original_content.length(), 0});
		_notification_visible = true;
	}

	void hide_notification() {
		if(_notification_visible) {
			set_contents(_original_content);
			set_cursor({row_length(0), 0});
			_notification_visible = false;
		}
	}

	void validate(const cppurses::Glyph_string& text) {
		if(_parse_success) {
			validated();
		}
	}

	void try_parse(const cppurses::Glyph_string& text) {
		_parse_success = false;
		uint32_t val;
		int num_chars;
		int scanf_res = std::sscanf(text.str().c_str(), "%8X%n", &val, &num_chars);
		bool one_element_scanned = (1 == scanf_res);
		bool acceptable_size = (num_chars >= _min_chars) &&
							   (num_chars <= _max_chars) &&
							   text.length() <= (size_t)_max_chars;

		_parse_success = acceptable_size && one_element_scanned;

		if(_parse_success) {
			_address = val;
			helpers::set_foreground(border, cppurses::Color::Green);
		} else {
			helpers::set_foreground(border, cppurses::Color::Yellow);
		}
		this->update();
	}



};


#endif //MICROMACHINE_EMU_ADDRESSINPUT_HPP
