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

	const std::array<exception, 22> exceptions_of_interest = {{
		exception::RESET,   exception::NMI,     exception::HARDFAULT, exception::SVCALL,  exception::PENDSV,
		exception::SYSTICK, exception::EXTI_00, exception::EXTI_01,   exception::EXTI_02, exception::EXTI_03,
		exception::EXTI_04, exception::EXTI_05, exception::EXTI_06,   exception::EXTI_07, exception::EXTI_08,
		exception::EXTI_09, exception::EXTI_10, exception::EXTI_11,   exception::EXTI_12, exception::EXTI_13,
		exception::EXTI_14, exception::EXTI_15,
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

		const exception current_active = _mcu.get_cpu().special_regs().interrupt_status_register().exception();
		for(exception exception : exceptions_of_interest) {
			const bool exception_is_active = _mcu.exceptions().is_active(exception);
			const bool exception_is_pending = _mcu.exceptions().is_pending(exception);
			uint32_t vector_table_offset = sizeof(uint32_t) * exception;
			uint32_t handler_address = _mcu.get_memory().read32(vector_table_offset) & ~1;
			const char* name = interrupt_names[exception];
			wchar_t symbol = exception_is_active ? L'⎆' : exception_is_pending ? L'⚑' : ' ';
			swprintf(_line_buffer, sizeof(_line_buffer), L"%02i %lc %08x %s\n"
				, exception
				, symbol
				, handler_address
				, name
			);
			if(exception == current_active) {
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

