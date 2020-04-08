/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_DISASMVIEW_HPP
#define MICROMACHINE_EMU_DISASMVIEW_HPP

#include <cppurses/widget/layouts/vertical.hpp>
#include <cpu.hpp>

#include "widgets/FoldableWidgetHeader.hpp"
#include "BreakpointManager.hpp"

class DisasmView : public cppurses::layout::Vertical {
private:
	cpu& _cpu;
	BreakpointManager& _breakpoint_manager;
	uint32_t _page_address;
	uint32_t _page_end;
	uint32_t _cursor_address;
	FoldableWidgetHeader& _header{this->make_child<FoldableWidgetHeader>("Disassembly")};
	cppurses::Text_display& _text{this->make_child<cppurses::Text_display>()};

public:
	DisasmView(cpu& cpu, BreakpointManager& breakpoint_manager)
		: _cpu(cpu)
		, _breakpoint_manager(breakpoint_manager)
		, _page_address(0)
		, _page_end(0)
		, _cursor_address(0) {

		_text.set_alignment(cppurses::Alignment::Left);
		_text.clear();

		// since this component draws some breakpoint state, it must
		// refresh also when a change in the breakpoints happen.
		_breakpoint_manager.changed.connect([this](){
			update();
		});
	}

	bool focus_in_event() override {
		_header.select();
		return Widget::focus_in_event();
	}

	bool focus_out_event() override {
		_header.unselect();
		return Widget::focus_out_event();
	}

	bool paint_event() override {
		//_cursor_address = _cpu.regs().get_pc();
		render();
		return Widget::paint_event();
	}

private:

	uint32_t previous_nth_address(size_t n) {
		uint32_t address = _cursor_address;
		for(size_t i = 0; i < n; i++) {
			address = previous_instruction_address(address);
		}
		return address;
	}

	uint32_t previous_cursor_address() {
		return previous_instruction_address(_cursor_address);
	}

	uint32_t previous_instruction_address(uint32_t address) {
		uint32_t look_behind_adress = address - sizeof(instruction_pair);
		instruction_pair look_behind = _cpu.fetch_instruction(look_behind_adress);
		if(look_behind.is_wide()) {
			return look_behind_adress;
		} else {
			return look_behind_adress + sizeof(uint16_t);
		}
	}

	uint32_t next_cursor_address() {
		return next_intruction_address(_cursor_address);
	}

	uint32_t next_intruction_address(uint32_t address) {
		return address + _cpu.fetch_instruction(address).size();
	}

	void move_cursor_up() {
		const uint32_t lines_per_page = _text.height();
		uint32_t new_address = previous_cursor_address();
		if(new_address < _page_address) {
			_page_address = previous_nth_address(lines_per_page);
		}
		_cursor_address = new_address;
		update();
	}

	void move_cursor_down() {
		uint32_t new_address = next_cursor_address();
		if(new_address >= _page_end) {
			_page_address = new_address;
		}
		_cursor_address = new_address;
		update();
	}

	bool key_press_event(const cppurses::Key::State& keyboard) override {
		if(keyboard.key == cppurses::Key::Arrow_down) {
			move_cursor_down();
			return true;
		} else if(keyboard.key == cppurses::Key::Arrow_up) {
			move_cursor_up();
			return true;
		} else {
			return cppurses::layout::Vertical::key_press_event(keyboard);
		}
	}

	void render() {

		static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> str_converter;
		_text.clear();
		const uint32_t lines_per_page = _text.height();
		uint32_t pc = _cpu.regs().get_pc();
		uint32_t address = pc;
		if(address > _page_end || address < _page_address) {
			_page_address = address;
		} else {
			address = _page_address;
		}
		wchar_t line[128] = {0};

		for(size_t i = 0; i < lines_per_page; i++) {
			auto instruction = _cpu.fetch_instruction(address);
			std::string disasm = disasm::disassemble_instruction(instruction, address);
			swprintf(line, sizeof(line), L" %08x│ %s\n", address, disasm.c_str());
			auto maybe_breakpoint = _breakpoint_manager.breakpoint_at(address);
			bool breakpoint_present = maybe_breakpoint.second;
			if(breakpoint_present) {
				//_text.insert_brush.add_attributes(cppurses::Attribute::Underline);
				if(maybe_breakpoint.first->second.enabled()) {
					line[0] = L'•';
				} else {
					line[0] = L'◦';
				}
			}
			if(address == pc) {
				_text.insert_brush.add_attributes(cppurses::Attribute::Standout);
				_text.append(line);
			} else {
				_text.append(line);
			}
			_text.insert_brush.clear_attributes();
			address += instruction.size();
		}
		_page_end = address;

	}

};

#endif //MICROMACHINE_EMU_DISASMVIEW_HPP
