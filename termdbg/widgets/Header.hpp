/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_HEADER_HPP
#define MICROMACHINE_EMU_HEADER_HPP

#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/label.hpp>
#include <cppurses/widget/widgets/fixed_height.hpp>

class Header : public cppurses::layout::Vertical {
private:
	cppurses::Label& _title;
	cppurses::Fixed_height& _line_break{this->make_child<cppurses::Fixed_height>(1)};
public:
	Header(cppurses::Glyph_string title_text)
		: _title{this->make_child<cppurses::Label>(std::move(title_text))} {
		_line_break.wallpaper = L'─';
		height_policy.fixed(2);
		_title.brush.add_attributes(cppurses::Attribute::Bold);
		_line_break.brush.set_foreground(cppurses::Color::Dark_gray);
	}

	void focus() {
		_title.brush.set_background(cppurses::Color::Yellow);
		_title.brush.set_foreground(cppurses::Color::Black);
		_title.set_contents(_title.contents()); // trick to force update
	}

	void unfocus() {
		_title.brush.set_background(cppurses::Color::Black);
		_title.brush.set_foreground(cppurses::Color::White);
		_title.set_contents(_title.contents()); // trick to force update
	}
};

#endif //MICROMACHINE_EMU_HEADER_HPP
