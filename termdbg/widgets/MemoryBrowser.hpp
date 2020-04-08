/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_MEMORYBROWSER_HPP
#define MICROMACHINE_EMU_MEMORYBROWSER_HPP


#include <cppurses/widget/layouts/vertical.hpp>
#include <cpu.hpp>

#include "widgets/HideableElement.hpp"
#include "widgets/CommandInput.hpp"
#include "widgets/MemorySegmentList.hpp"
#include "widgets/MemorySegmentInfo.hpp"
#include "widgets/MemoryView.hpp"
#include "widgets/InfoTable.hpp"
#include "widgets/Helpers.hpp"

class MemoryBrowser : public cppurses::layout::Vertical {
public:
	cpu& _cpu;
	FoldableWidgetHeader& _header;
	HidableElement<CommandInput> _menu;
	HidableElement<MemorySegmentList> _memory_segments;
	MemorySegmentInfo& _segment_info;
	MemoryView& _memview;
	InfoTable& _info_table {make_child<InfoTable>(4)};

	MemoryBrowser(cpu& cpu)
	 : _cpu(cpu)
	 , _header(this->make_child<FoldableWidgetHeader>("Memory"))
	 , _menu(this, 1, cppurses::Glyph_string("command..."))
	 , _memory_segments(this, 2, cpu.mem())
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


		_menu.getWidget().register_command("goto", std::regex("(goto) ([a-f0-9]+)"));
		_menu.getWidget().command_parsed.connect([this](const std::string& command, const std::vector<std::string>& args){
			if(process_command(command, args)) {
				close_menu();
			}
		});

		_menu.getWidget().input_aborted.connect([this](){
			close_menu();
		});

		key_pressed.connect([this](cppurses::Key::Code key){
			if(key == cppurses::Key::o) {
				open_menu();
			}
		});

		_memory_segments.getWidget().segment_selected.connect([this](size_t segment_index){
			_memory_segments.hide();
			auto* segment = &_cpu.mem().regions()[segment_index];
			_memview.set_segment(segment);
			_segment_info.set_segment(segment);
			_info_table.set_segment(segment);
		});

		_memory_segments.getWidget().input_cancelled.connect([this](){
			_memory_segments.hide();
		});

		if(!_cpu.mem().regions().empty()) {
			auto* segment = &_cpu.mem().regions()[0];
			_memview.set_segment(segment);
			_segment_info.set_segment(segment);
			_info_table.set_segment(segment);
		}
	}

	void close_menu() {
		_menu.hide();
	}

	void open_menu() {
		_menu.show();
	}

	void update() override {
		_info_table.set_segment_offset(_memview.cursor_offset());
		_memview.update();
		_info_table.update();
		return cppurses::layout::Vertical::update();
	}

	bool process_command(const std::string& command, const std::vector<std::string>& args) {
		if(command == "goto" && args.size() > 0) {
			uint32_t address = 0;
			if(1 == sscanf(args[0].data(), "%x", &address)) {
				const memory_mapping* segment = _cpu.mem().find_const_region(address);
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
		}
		return true;
	}

	bool paint_event() override {
		render();
		return Widget::paint_event();
	}

	void render() {
		char buff[32];
		snprintf(buff, 32, "%08x", _memview.cursor_offset());
		_info_table.get_cell_at<RightAlignedText>(0,0).set_contents(buff);
	}

	bool key_press_event(const cppurses::Key::State& keyboard) override {
		auto key = keyboard.key;

		if(key == cppurses::Key::Code::o) {
			_menu.show();
			update();
			return true;
		} if(key == cppurses::Key::Code::m) {
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
};

#endif //MICROMACHINE_EMU_MEMORYBROWSER_HPP
