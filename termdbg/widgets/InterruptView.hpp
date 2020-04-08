/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_INTERRUPTVIEW_HPP
#define MICROMACHINE_EMU_INTERRUPTVIEW_HPP

#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/text_display.hpp>
#include <exception_state.hpp>

namespace {
	const char* interrupt_names[] = {
			"invalid",
			"Reset",
			"NMI",
			"Hard Fault",
			"reserved_0",
			"reserved_1",
			"reserved_2",
			"reserved_3",
			"reserved_4",
			"reserved_5",
			"reserved_6",
			"SVC",
			"reserved_7",
			"reserved_8",
			"Pend SV",
			"Systick",
			"External Interrupt 0",
			"External Interrupt 1",
			"External Interrupt 2",
			"External Interrupt 3",
			"External Interrupt 4",
			"External Interrupt 5",
			"External Interrupt 6",
			"External Interrupt 7",
			"External Interrupt 8",
			"External Interrupt 9",
			"External Interrupt 10",
			"External Interrupt 11",
			"External Interrupt 12",
			"External Interrupt 13",
			"External Interrupt 14",
			"External Interrupt 15"

	};

	const std::array<size_t, 22> interrupts_of_interest = {{
		1,2,3,11,
		14,15,16,17,
		18,19,20,21,
		22,23,24,25,
		26,27,28,29,
		30,31
	}};

}

class InterruptView : public cppurses::layout::Vertical {
private:
	const exception_state_vector& _exception_state;
	const ipsr_reg& _ipsr_reg;
	FoldableWidgetHeader& _header;
	cppurses::Text_display& _interrupt_list;

public:
	InterruptView(const exception_state_vector& exception_state, const ipsr_reg& ipsr_reg)
		: _exception_state(exception_state)
		, _ipsr_reg(ipsr_reg)
		, _header(make_child<FoldableWidgetHeader>("Interrupts"))
		, _interrupt_list(make_child<cppurses::Text_display>())
		{}


	bool paint_event() override {
		render();
		return Widget::paint_event();
	}

	void render() {
		_interrupt_list.clear();
		wchar_t line[128];

		for(size_t interrupt_number : interrupts_of_interest) {
			const exception::Type current_active = _ipsr_reg.exception_num();
			const exception_state& state = _exception_state.at(interrupt_number);
			const char* name = interrupt_names[interrupt_number];
			wchar_t symbol = state.is_active() ? L'⎆' : state.is_pending() ? L'⚑' : ' ';
			swprintf(line, sizeof(line), L"%02i %lc %s\n"
				, interrupt_number
				, symbol
				, name
			);
			if(interrupt_number == current_active) {
				_interrupt_list.insert_brush.add_attributes(cppurses::Attribute::Standout);
			}
			_interrupt_list.append(line);
			_interrupt_list.insert_brush.clear_attributes();
		}

	}

	bool focus_in_event() override {
		_header.select();
		update();
		return Widget::focus_in_event();
	}

	bool focus_out_event() override {
		_header.unselect();
		update();
		return Widget::focus_out_event();
	}
};

#endif //MICROMACHINE_EMU_INTERRUPTVIEW_HPP
