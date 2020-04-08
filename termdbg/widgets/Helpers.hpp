/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_HELPERS_HPP
#define MICROMACHINE_EMU_HELPERS_HPP


#include <cppurses/widget/border.hpp>

namespace helpers {
	void set_background(cppurses::Border& border, cppurses::Color color) {
		border.segments.north.brush.set_background(color);
		border.segments.south.brush.set_background(color);
		border.segments.east.brush.set_background(color);
		border.segments.west.brush.set_background(color);
		border.segments.north_east.brush.set_background(color);
		border.segments.north_west.brush.set_background(color);
		border.segments.south_east.brush.set_background(color);
		border.segments.south_west.brush.set_background(color);
	}

	void set_foreground(cppurses::Border& border, cppurses::Color color) {
		border.segments.north.brush.set_foreground(color);
		border.segments.south.brush.set_foreground(color);
		border.segments.east.brush.set_foreground(color);
		border.segments.west.brush.set_foreground(color);
		border.segments.north_east.brush.set_foreground(color);
		border.segments.north_west.brush.set_foreground(color);
		border.segments.south_east.brush.set_foreground(color);
		border.segments.south_west.brush.set_foreground(color);
	}

}

#endif //MICROMACHINE_EMU_HELPERS_HPP
