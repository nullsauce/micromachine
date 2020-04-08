/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_BREAKPOINTMANAGERVIEW_HPP
#define MICROMACHINE_EMU_BREAKPOINTMANAGERVIEW_HPP


#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/text_display.hpp>

#include "BreakpointManager.hpp"
#include "widgets/HideableElement.hpp"
#include "widgets/AddressInput.hpp"
#include "widgets/Header.hpp"

class BreakPointManagerView : public cppurses::layout::Vertical {
private:
	size_t _selected_index;
	BreakpointManager& _breakpoint_manager;
	Header& _header;
	HidableElement<AddressInput> _new_breakpoint_address;
	cppurses::Text_display& _breakpoint_list;
	bool _active;
	size_t _scroll_offset;

public:
	BreakPointManagerView(BreakpointManager& breakpoint_manager)
		: _selected_index(0)
		, _breakpoint_manager(breakpoint_manager)
		, _header(this->make_child<Header>("Breakpoints"))
		, _new_breakpoint_address(this, 1, "address")
		, _breakpoint_list(this->make_child<cppurses::Text_display>())
		, _active(false)
		, _scroll_offset(0)
	{
		_breakpoint_manager.create_breakpoint_at(0xca);
		_breakpoint_manager.create_breakpoint_at(0xce);
		_breakpoint_manager.create_breakpoint_at(0xc8);
		_breakpoint_manager.toggle_breakpoint_at(0xc8);
		_breakpoint_manager.create_breakpoint_at(0x1234);
		_breakpoint_manager.create_breakpoint_at(0x4000);
		_breakpoint_manager.create_breakpoint_at(0x0);

		_new_breakpoint_address.getWidget().input_aborted.connect([this](){
			_new_breakpoint_address.hide();
			cppurses::Focus::set_focus_to(*this);
		});

		_new_breakpoint_address.getWidget().validated.connect([this](){
			if(_new_breakpoint_address.getWidget().parse_success()) {
				uint32_t address = _new_breakpoint_address.getWidget().address();
				_breakpoint_manager.create_breakpoint_at(address);
			}
			_new_breakpoint_address.hide();
			cppurses::Focus::set_focus_to(*this);
			update();
		});

	}

	bool paint_event() override {
		render();
		return Widget::paint_event();
	}

	void render() {
		_breakpoint_list.clear();
		if(_breakpoint_manager.breakpoint_count() == 0) {
			_breakpoint_list.set_alignment(cppurses::Alignment::Center);
			_breakpoint_list.brush.set_foreground(cppurses::Color::Dark_gray);
			_breakpoint_list.append("No breakpoints");
			_breakpoint_list.insert_brush.clear_attributes();
		} else {
			_breakpoint_list.brush.set_foreground(cppurses::Color::White);
			_breakpoint_list.set_alignment(cppurses::Alignment::Left);

			wchar_t line[128] = {0};

			size_t index = _scroll_offset;
			auto it = _breakpoint_manager.begin();
			std::advance(it, _scroll_offset);
			const auto first_visible_breakpoint = it;
			std::advance(it, num_visible_breakpoints());
			const auto after_last_visible_breakpoint = it;

			auto bp_it = first_visible_breakpoint;
			while(bp_it != after_last_visible_breakpoint) {

				const std::pair<uint32_t, Breakpoint>& breakpoint = *bp_it;
				bool is_selected_breakpoint = _active && index == _selected_index;
				if(breakpoint.second.reached()) {
					_breakpoint_list.insert_brush.add_attributes(cppurses::Attribute::Standout);
				}

				if(is_selected_breakpoint) {
					//_breakpoint_list.insert_brush.add_attributes(cppurses::Attribute::Underline);
				}
				wchar_t indicator = breakpoint.second.reached() ? L'\x1f846' : breakpoint.second.enabled() ? L'✓' :
				L'✗';
				swprintf(line, sizeof(line), L"%lc %08x %lc\n"
					, indicator
					, breakpoint.first
					, is_selected_breakpoint ? L'◄' : L' '
				);
				_breakpoint_list.append(line);
				_breakpoint_list.insert_brush.clear_attributes();

				std::advance(bp_it, 1);
				index++;
			}
		}
	}

	bool focus_in_event() override {
		_active = true;
		_header.focus();
		update();
		return Widget::focus_in_event();
	}

	bool focus_out_event() override {
		_active = false;
		_header.unfocus();
		update();
		return Widget::focus_out_event();
	}

	// returns the maximum number of visible breakpoints.
	// The total breakpoint count, if smaller than the number of visible lines
	// The number of visible lines otherwise
	size_t num_visible_breakpoints() const {
		return std::min(_breakpoint_list.height(), _breakpoint_manager.breakpoint_count());
	}

	bool key_press_event(const cppurses::Key::State& keyboard) override {
		auto key = keyboard.key;

		if(key == cppurses::Key::Code::n) {
			_new_breakpoint_address.getWidget().enable_placeholder();
			_new_breakpoint_address.show();
			update();
			return true;

		} else if(key == cppurses::Key::Arrow_down) {
			if(_selected_index + 1 < _breakpoint_manager.breakpoint_count()) {
				_selected_index++;
				// adjust the scroll window if overflow downward
				if(_selected_index >= _scroll_offset + num_visible_breakpoints()) {
					_scroll_offset = std::max(1 + ((int)_selected_index - (int)num_visible_breakpoints()), 0);
				}
			}
			update();
			return true;

		} else if(key == cppurses::Key::Arrow_up) {
			if(_selected_index > 0) {
				_selected_index--;
				// adjust the scroll window if overflow upward
				if(_selected_index < _scroll_offset) {
					_scroll_offset = std::min((int)_breakpoint_manager.breakpoint_count() - (int)num_visible_breakpoints(), (int)_selected_index);
				}
			}
			update();
			return true;

		} else if(key == cppurses::Key::Space) {
			uint32_t address = _breakpoint_manager.nth_breakpoint(_selected_index).address();
			_breakpoint_manager.toggle_breakpoint_at(address);
			update();
			return true;

		} else if(key == cppurses::Key::Delete_character) {
			uint32_t address = _breakpoint_manager.nth_breakpoint(_selected_index).address();
			_breakpoint_manager.destroy_breakpoint_at(address);
			_selected_index = std::max(0, (int)_selected_index-1);
			update();
			return true;
		}
		return cppurses::layout::Vertical::key_press_event(keyboard);
	}
};

#endif //MICROMACHINE_EMU_BREAKPOINTMANAGERVIEW_HPP
