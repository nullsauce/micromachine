/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once


#include <cppurses/widget/layouts/vertical.hpp>
#include <mcu.hpp>

#include "FormatedText.hpp"
#include "widgets/CommandInput.hpp"
#include "widgets/Helpers.hpp"
#include "widgets/HideableElement.hpp"
#include "widgets/InfoTable.hpp"
#include "widgets/MemorySegmentInfo.hpp"
#include "widgets/MemorySegmentList.hpp"
#include "widgets/MemoryView.hpp"

class MemoryBrowser : public cppurses::layout::Vertical {
public:
	memory& _memory;
	FoldableWidgetHeader& _header;
	HidableElement<AddressInput> _goto_input;
	HidableElement<MemorySegmentList> _memory_segments;
	MemorySegmentInfo& _segment_info;
	MemoryView& _memview;
	InfoTable& _info_table {make_child<InfoTable>(4)};
	const cppurses::Glyph_string _help_content = FormatedText()
		.write("Goto address").write_color_f("................ ", cppurses::Color::Dark_gray)
		.write_color_fb("G", cppurses::Color::Yellow, cppurses::Color::Blue)
		.write("\n")
		.write("Browse segments").write_color_f("............. ", cppurses::Color::Dark_gray)
		.write_color_fb("S", cppurses::Color::Yellow, cppurses::Color::Blue)
		.write("\n")
		.write("Move cursor").write_color_f("................. ", cppurses::Color::Dark_gray)
		.write_color_fb("Arrows", cppurses::Color::Yellow, cppurses::Color::Blue)
		.write("\n")
		.write("Cancel text input").write_color_f("........... ", cppurses::Color::Dark_gray)
		.write_color_fb("Esc", cppurses::Color::Yellow, cppurses::Color::Blue)
		.write("\n")
		.write("Hide").write_color_f("........................ ", cppurses::Color::Dark_gray)
		.write_color_fb("PageUp", cppurses::Color::Yellow, cppurses::Color::Blue)
		.write("\n")
		.write("Show").write_color_f("........................ ", cppurses::Color::Dark_gray)
		.write_color_fb("PageDown", cppurses::Color::Yellow, cppurses::Color::Blue)
		.str();


	MemoryBrowser(memory& memory)
	 : _memory(memory)
	 , _header(this->make_child<FoldableWidgetHeader>("Memory"))
	 , _goto_input(this, 1)
	 , _memory_segments(this, 2, _memory)
	 , _segment_info(make_child<MemorySegmentInfo>())
 	 , _memview(make_child<MemoryView>()) {
		_info_table.height_policy.fixed(6);
		_info_table.make_cell<RightAlignedText>(".");
	 	_info_table.make_cell<RightAlignedText>("8 bits");
	 	_info_table.make_cell<RightAlignedText>("16 bits");
	 	_info_table.make_cell<RightAlignedText>("32 bits");
	 	_info_table.make_cell<RightAlignedText>("LE uns.");
	 	_info_table.make_cell<RightAlignedText>("u8");
	 	_info_table.make_cell<RightAlignedText>("u16");
	 	_info_table.make_cell<RightAlignedText>("u32");
		_info_table.make_cell<RightAlignedText>("LE sig.");
	 	_info_table.make_cell<RightAlignedText>("i8");
	 	_info_table.make_cell<RightAlignedText>("i16");
		_info_table.make_cell<RightAlignedText>("i32");
		_info_table.make_cell<RightAlignedText>("BE uns.");
	 	_info_table.make_cell<RightAlignedText>("u8");
	 	_info_table.make_cell<RightAlignedText>("u16");
	 	_info_table.make_cell<RightAlignedText>("u32");
		_info_table.make_cell<RightAlignedText>("BE sig.");
	 	_info_table.make_cell<RightAlignedText>("i8");
	 	_info_table.make_cell<RightAlignedText>("i16");
		_info_table.make_cell<RightAlignedText>("i32");



		_goto_input.getWidget().input_aborted.connect([this](){
			_goto_input.hide();
			cppurses::Focus::set_focus_to(*this);
		});

		_goto_input.getWidget().validated.connect([this](){
			if(_goto_input.getWidget().parse_success()) {
				uint32_t address = _goto_input.getWidget().address();
				if(goto_address(address)) {
					_goto_input.hide();
					cppurses::Focus::set_focus_to(*this);
				} else {
					_goto_input.getWidget().mark_invalid();
				}
			}
			update();
		});


		_memory_segments.getWidget().height_policy.min_size(_memory.regions().size() + 2);

		_memory_segments.getWidget().segment_selected.connect([this](size_t segment_index){
			_memory_segments.hide();
			auto* segment = &_memory.regions()[segment_index];
			_memview.set_segment(segment);
			_segment_info.set_segment(segment);
			_info_table.set_segment(segment);
			cppurses::Focus::set_focus_to(*this);
		});

		_memory_segments.getWidget().input_cancelled.connect([this](){
			_memory_segments.hide();
			cppurses::Focus::set_focus_to(*this);
		});

		if(!_memory.regions().empty()) {
			auto* segment = &_memory.regions()[0];
			_memview.set_segment(segment);
			_segment_info.set_segment(segment);
			_info_table.set_segment(segment);
		}
	}

	const cppurses::Glyph_string& help_content() const {
		return _help_content;
	}

	void update() override {
		_info_table.set_segment_offset(_memview.cursor_offset());
		_memview.update();
		_info_table.update();

		return cppurses::layout::Vertical::update();
	}

	bool paint_event() override {
		render();
		return Widget::paint_event();
	}

	void render() {
		static char buff[32];
		snprintf(buff, 32, "%08x", _memview.cursor_offset());
		_info_table.get_cell_at<RightAlignedText>(0,0).set_contents(buff);
	}

	bool key_press_event(const cppurses::Key::State& keyboard) override {
		auto key = keyboard.key;

		if(key == cppurses::Key::Code::g) {
			_goto_input.show();
			update();
			return true;
		} if(key == cppurses::Key::Code::s) {
			_memory_segments.show();
			update();
			return true;
		} else if(key == cppurses::Key::Code::Arrow_up) {
			//_view.scroll_line_down();
			_memview.move_cursor_up();
			update();
			return true;
		} else if(key == cppurses::Key::Code::Arrow_down) {
			//_view.scroll_line_up();
			_memview.move_cursor_down();
			update();
			return true;
		} else if(key == cppurses::Key::Code::Arrow_left) {
			//_view.scroll_line_up();
			_memview.move_cursor_left();
			update();
			return true;
		} else if(key == cppurses::Key::Code::Arrow_right) {
			//_view.scroll_line_up();
			_memview.move_cursor_right();
			update();
			return true;
		} else if(key == cppurses::Key::Code::Next_page) {
			//_view.scroll_page_down();
			height_policy.expanding(0);
			_header.maximize();
			update();
			return true;
		} else if(key == cppurses::Key::Code::Previous_page) {
			//_view.scroll_page_up();
			height_policy.fixed(3);
			_header.minimize();
			update();
			return true;
		}
		return _memview.key_press_event(keyboard);
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

	bool goto_address(uint32_t address) {
		const memory_mapping* segment = _memory.find_const_region(address);
		if(segment) {
			_memview.set_segment(segment);
			_segment_info.set_segment(segment);
			_info_table.set_segment(segment);
			_memview.navigate_to_address(address);
			return true;
		} else {
			return false;
		}
	}
};

