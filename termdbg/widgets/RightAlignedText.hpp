

#pragma once

#include <cppurses/widget/widgets/text_display.hpp>

class RightAlignedText : public cppurses::Text_display {
public:
	RightAlignedText(const cppurses::Glyph_string& text) : cppurses::Text_display(text) {
		set_alignment(cppurses::Alignment::Right);
	}
};


