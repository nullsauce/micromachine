/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_REGISTERSVIEW_HPP
#define MICROMACHINE_EMU_REGISTERSVIEW_HPP

#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/label.hpp>
#include <cppurses/widget/widgets/fixed_height.hpp>

#include <cpu.hpp>
#include <cstring>

#include "widgets/FoldableWidgetHeader.hpp"

class RegistersView : public cppurses::layout::Vertical {
private:
	FoldableWidgetHeader& _header;
	cpu& _cpu;
	uint32_t _previous_values[16];
	const char* const register_names[16] = {
		"R0 ","R1 ","R2 ","R3 ",
		"R4 ","R5 ","R6 ","R7 ",
		"R8 ","R9 ","R10","R11",
		"R12","SP ","LR ","PC "
	};
	char _line_buffer[64] = {0};
public:

	cppurses::Text_display& regs_text{this->make_child<cppurses::Text_display>()};

	RegistersView(cpu& cpu)
		: _header(make_child<FoldableWidgetHeader>("Registers"))
		, _cpu(cpu) {
		memset(_previous_values, 0, sizeof(_previous_values));
		regs_text.set_alignment(cppurses::Alignment::Left);
		regs_text.clear();
	}

	bool paint_event() override {
		render();
		return Widget::paint_event();
	}

	void render() {
		regs_text.clear();
		for(int i = 0; i < 16; i++) {

			uint32_t actual_reg_value = _cpu.regs().get(i);
			uint32_t previous_reg_value = _previous_values[i];
			bool changed = previous_reg_value != actual_reg_value;
			_previous_values[i] = actual_reg_value;
			sprintf(_line_buffer, "%s %08x\n", register_names[i], actual_reg_value);
			if(changed) {
				regs_text.insert_brush.add_attributes(cppurses::Attribute::Standout);
				regs_text.append(_line_buffer);
				regs_text.insert_brush.remove_attributes(cppurses::Attribute::Standout);
			} else {
				regs_text.append(_line_buffer);
			}
		}
	}

	bool focus_in_event() override {
		_header.select();
		this->update();
		return Widget::focus_in_event();
	}

	bool focus_out_event() override {
		_header.unselect();
		this->update();
		return Widget::focus_out_event();
	}
};

#endif //MICROMACHINE_EMU_REGISTERSVIEW_HPP
