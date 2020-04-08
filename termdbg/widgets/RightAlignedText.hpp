/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_RIGHTALIGNEDTEXT_HPP
#define MICROMACHINE_EMU_RIGHTALIGNEDTEXT_HPP

#include <cppurses/widget/widgets/text_display.hpp>

class RightAlignedText : public cppurses::Text_display {
public:
	RightAlignedText(const cppurses::Glyph_string& text) : cppurses::Text_display(text) {
		set_alignment(cppurses::Alignment::Right);
	}
};


#endif //MICROMACHINE_EMU_RIGHTALIGNEDTEXT_HPP
