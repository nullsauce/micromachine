/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_TOPMENUVIEW_HPP
#define MICROMACHINE_EMU_TOPMENUVIEW_HPP

#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/widgets/label.hpp>

class TopMenuView : public cppurses::layout::Horizontal {
private:
	cppurses::Label& _help_label;

public:
	TopMenuView()
		: _help_label(this->make_child<cppurses::Label>("label"))
	{}
};

#endif //MICROMACHINE_EMU_TOPMENUVIEW_HPP
