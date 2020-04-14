/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_FOLDABLEWIDGETHEADER_HPP
#define MICROMACHINE_EMU_FOLDABLEWIDGETHEADER_HPP

#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/label.hpp>
#include <cppurses/widget/widgets/fixed_height.hpp>

class FoldableWidgetHeader : public cppurses::layout::Vertical {
private:
	cppurses::Glyph_string _title;
	bool _maximized;
	cppurses::Label& _title_label;
	cppurses::Fixed_height& _line_break{this->make_child<cppurses::Fixed_height>(1)};

public:
	FoldableWidgetHeader(cppurses::Glyph_string title)
		: _title(std::move(title))
		, _maximized(true)
		, _title_label{this->make_child<cppurses::Label>()} {
		_line_break.wallpaper = L'─';

		_line_break.brush.set_foreground(cppurses::Color::Dark_gray);
		maximize();
	}

	bool paint_event() override {
		render();
		return Widget::paint_event();
	}

	void render() {
		_title_label.clear();
		_title_label.append(_maximized ? L'\x1F4C2' : L'\x1F4C1');
		_title_label.append(L' ');
		_title_label.append(_title);
	}

	void select() {
		_title_label.brush.set_background(cppurses::Color::Yellow);
		_title_label.brush.set_foreground(cppurses::Color::Black);
		update();
	}

	void unselect() {
		_title_label.brush.set_background(cppurses::Color::Black);
		_title_label.brush.set_foreground(cppurses::Color::White);
		update();
	}

	void minimize() {
		height_policy.fixed(1);
		_maximized = false;
		update();
	}

	void maximize() {
		height_policy.fixed(2);
		_maximized = true;
		update();
	}
};

#endif //MICROMACHINE_EMU_FOLDABLEWIDGETHEADER_HPP
