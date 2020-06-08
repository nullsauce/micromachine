

#pragma once


#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/text_display.hpp>

#include "BreakpointManager.hpp"
#include "widgets/HideableElement.hpp"
#include "widgets/AddressInput.hpp"
#include "widgets/FoldableWidgetHeader.hpp"

class BreakPointManagerView : public cppurses::layout::Vertical {
private:
	size_t _selected_index;
	BreakpointManager& _breakpoint_manager;
	FoldableWidgetHeader& _header;
	HidableElement<AddressInput> _new_breakpoint_address;
	cppurses::Text_display& _breakpoint_list;
	bool _active;
	size_t _scroll_offset;
	const cppurses::Glyph_string _help_content = FormatedText()
		.write("Add a new breakpoint").write_color_f("........ ", cppurses::Color::Dark_gray)
		.write_color_fb("N", cppurses::Color::Yellow, cppurses::Color::Blue)
		.write("\n")
		.write("Toggle breakpoint").write_color_f("........... ", cppurses::Color::Dark_gray)
		.write_color_fb("Space", cppurses::Color::Yellow, cppurses::Color::Blue)
		.write("\n")
		.write("Delete breakpoint").write_color_f("........... ", cppurses::Color::Dark_gray)
		.write_color_fb("Del", cppurses::Color::Yellow, cppurses::Color::Blue)
		.write("\n")
		.write("Navigate the list with").write_color_f("...... ", cppurses::Color::Dark_gray)
		.write_color_fb("Up/Down", cppurses::Color::Yellow, cppurses::Color::Blue)
		.write("\n")
		.write("Cancel text input").write_color_f("........... ", cppurses::Color::Dark_gray)
		.write_color_fb("Esc", cppurses::Color::Yellow, cppurses::Color::Blue)
		.str();
	wchar_t _line_buffer[128] = {0};

public:
	BreakPointManagerView(BreakpointManager& breakpoint_manager)
		: _selected_index(0)
		, _breakpoint_manager(breakpoint_manager)
		, _header(this->make_child<FoldableWidgetHeader>("Breakpoints"))
		, _new_breakpoint_address(this, 1)
		, _breakpoint_list(this->make_child<cppurses::Text_display>())
		, _active(false)
		, _scroll_offset(0)
	{

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

	const cppurses::Glyph_string& help_content() const {
		return _help_content;
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
				swprintf(_line_buffer, sizeof(_line_buffer), L"%lc %08x %lc\n"
					, indicator
					, breakpoint.first
					, is_selected_breakpoint ? L'◄' : L' '
				);
				_breakpoint_list.append(_line_buffer);
				_breakpoint_list.insert_brush.clear_attributes();

				std::advance(bp_it, 1);
				index++;
			}
		}
	}

	bool focus_in_event() override {
		_active = true;
		_header.select();
		update();
		return Widget::focus_in_event();
	}

	bool focus_out_event() override {
		_active = false;
		_header.unselect();
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

		} else if(key == cppurses::Key::Code::Next_page) {
			_header.maximize();
			height_policy.expanding(0);
			update();
			return true;

		} else if(key == cppurses::Key::Code::Previous_page) {
			_header.minimize();
			height_policy.fixed(2);
			update();
			return true;
		}
		return cppurses::layout::Vertical::key_press_event(keyboard);
	}
};

