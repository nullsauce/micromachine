/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once

#include "cpu.hpp"
#include "exception/exception.hpp"
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/text_display.hpp>
#include <exception/exception_vector.hpp>

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
			"EXTI 0",
			"EXTI 1",
			"EXTI 2",
			"EXTI 3",
			"EXTI 4",
			"EXTI 5",
			"EXTI 6",
			"EXTI 7",
			"EXTI 8",
			"EXTI 9",
			"EXTI 10",
			"EXTI 11",
			"EXTI 12",
			"EXTI 13",
			"EXTI 14",
			"EXTI 15"

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
	mcu& _mcu;
	FoldableWidgetHeader& _header;
	cppurses::Text_display& _interrupt_list;
	wchar_t _line_buffer[128] = {};

public:
	InterruptView(mcu& mcu)
		: _mcu(mcu)
		, _header(make_child<FoldableWidgetHeader>("Interrupts"))
		, _interrupt_list(make_child<cppurses::Text_display>())
		{}


	bool paint_event() override {
		render();
		return Widget::paint_event();
	}

	void render() {
		_interrupt_list.clear();


		const exception::Type current_active = _mcu.get_cpu().special_regs().interrupt_status_register().exception_num();
		for(size_t interrupt_number : interrupts_of_interest) {
			const exception::Type exception = exception::from_number(interrupt_number);
			const bool exception_is_active = _mcu.exceptions().is_active(exception);
			const bool exception_is_pending = _mcu.exceptions().is_pending(exception);
			uint32_t vector_table_offset = sizeof(uint32_t) * interrupt_number;
			uint32_t handler_address = _mcu.get_memory().read32(vector_table_offset) & ~1;
			const char* name = interrupt_names[interrupt_number];
			wchar_t symbol = exception_is_active ? L'⎆' : exception_is_pending ? L'⚑' : ' ';
			swprintf(_line_buffer, sizeof(_line_buffer), L"%02i %lc %08x %s\n"
				, interrupt_number
				, symbol
				, handler_address
				, name
			);
			if(interrupt_number == current_active) {
				_interrupt_list.insert_brush.add_attributes(cppurses::Attribute::Standout);
			}
			_interrupt_list.append(_line_buffer);
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

