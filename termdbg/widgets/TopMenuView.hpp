/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/widgets/label.hpp>
#include <FormatedText.hpp>


class TopMenuView : public cppurses::layout::Horizontal {
private:
	cppurses::Label& _about_label;
	cppurses::Label& _contextual_help;

public:
	TopMenuView()
		: _about_label(this->make_child<cppurses::Label>("micromachine debugger"))
		, _contextual_help(this->make_child<cppurses::Label>())
	{
		_contextual_help.set_alignment(cppurses::Alignment::Right);
		_contextual_help.set_contents(
		FormatedText()
			.write_color_fb("Tab", cppurses::Color::Yellow, cppurses::Color::Blue)
			.write("Switch window ")
			.str()
		);
	}

};

