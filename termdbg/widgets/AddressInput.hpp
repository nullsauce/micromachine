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
public:
	sig::Signal<void()> validated;

	AddressInput(cppurses::Glyph_string placeholder)
		: SingleLineTextInput(std::move(placeholder))
		, _address(0)
		, _parse_success(false) {
		text_changed.connect(std::bind(&AddressInput::try_parse, this, std::placeholders::_1));
		text_entered.connect(std::bind(&AddressInput::validate, this, std::placeholders::_1));
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

private:
	uint32_t _address;
	bool _parse_success;

	void validate(const cppurses::Glyph_string& text) {
		if(_parse_success) {
			validated();
		}
	}

	void try_parse(const cppurses::Glyph_string& text) {
		_parse_success = false;
		uint32_t val;
		int num_chars;
		if(1 == std::sscanf(text.str().c_str(), "%8X%n", &val, &num_chars) && num_chars == 8) {
			_parse_success = true;
			_address = val;
		} else {
			_parse_success = false;
		}

		if(_parse_success) {
			helpers::set_foreground(border, cppurses::Color::Green);
		} else {
			helpers::set_foreground(border, cppurses::Color::Yellow);
		}
		this->update();
	}
};


#endif //MICROMACHINE_EMU_ADDRESSINPUT_HPP
