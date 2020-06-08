#pragma once

#include <cppurses/widget/layouts/vertical.hpp>
#include <cpu.hpp>

#include "BreakpointManager.hpp"
#include "helpers/disassembler.hpp"
#include "instruction/instruction_pair.hpp"
#include "widgets/FoldableWidgetHeader.hpp"

class DisasmView : public cppurses::layout::Vertical {
private:
	mcu& _mcu;
	BreakpointManager& _breakpoint_manager;
	ExecutionController& _execution_controller;
	uint32_t _page_address;
	uint32_t _page_end;
	uint32_t _cursor_address;
	FoldableWidgetHeader& _header{this->make_child<FoldableWidgetHeader>("Disassembly")};
	cppurses::Text_display& _text{this->make_child<cppurses::Text_display>()};
	const cppurses::Glyph_string _help_content = FormatedText()
		.write("Resume execution").write_color_f("............ ", cppurses::Color::Dark_gray)
		.write_color_fb("R", cppurses::Color::Yellow, cppurses::Color::Blue)
		.write("\n")
		.write("Pause execution").write_color_f("............. ", cppurses::Color::Dark_gray)
		.write_color_fb("P", cppurses::Color::Yellow, cppurses::Color::Blue)
		.write("\n")
		.write("Single step").write_color_f("................. ", cppurses::Color::Dark_gray)
		.write_color_fb("S", cppurses::Color::Yellow, cppurses::Color::Blue)
		.write("\n")
		.write("Reset").write_color_f("....................... ", cppurses::Color::Dark_gray)
		.write_color_fb("Ctrl+R", cppurses::Color::Yellow, cppurses::Color::Blue)
		.str();
	wchar_t _line_buffer[256] = {0};

public:
	DisasmView(mcu& mcu, BreakpointManager& breakpoint_manager, ExecutionController& _execution_controller)
		: _mcu(mcu)
		, _breakpoint_manager(breakpoint_manager)
		, _execution_controller(_execution_controller)
		, _page_address(0)
		, _page_end(0)
		, _cursor_address(0) {

		_text.disable_word_wrap();
		_text.set_alignment(cppurses::Alignment::Left);
		_text.clear();

		// since this component draws some breakpoint state, it must
		// refresh also when a change in the breakpoints happen.
		_breakpoint_manager.changed.connect([this](){
			update();
		});

	}

	const cppurses::Glyph_string& help_content() const {
		return _help_content;
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
		render();
		return Widget::paint_event();
	}

	void update() override {
		for(auto w : children.get_descendants()) {
			w->update();
		}
		Widget::update();
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
		instruction_pair look_behind = _mcu.get_memory().read32_unchecked(look_behind_adress);
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
		instruction_pair instruction_pair = _mcu.get_memory().read32_unchecked(address);
		return address + instruction_pair.size();
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
			return _execution_controller.key_press_event(keyboard);
		}
	}

	void render() {

		_text.clear();
		const uint32_t lines_per_page = _text.height();
		uint32_t pc = _mcu.get_cpu().regs().pc();
		uint32_t address = pc;
		if(address > _page_end || address < _page_address) {
			_page_address = address;
		} else {
			address = _page_address;
		}

		for(size_t i = 0; i < lines_per_page; i++) {

			instruction_pair instruction = _mcu.get_memory().read32_unchecked(address);
			std::string disasm = disassembler::disassemble_instruction(instruction, address);
			auto maybe_breakpoint = _breakpoint_manager.breakpoint_at(address);
			bool breakpoint_present = maybe_breakpoint.second;
			wchar_t breakpoint_symbol = L' ';
			if(breakpoint_present) {
				breakpoint_symbol = maybe_breakpoint.first->second.enabled() ? L'•' : L'◦';
			}

			std::swprintf(_line_buffer, sizeof(_line_buffer), L"%lc %08x│ %s\n"
				, breakpoint_symbol
				, address
				, disasm.c_str()
			);
			if(address == pc) {
				_text.insert_brush.add_attributes(cppurses::Attribute::Standout);
				_text.append(_line_buffer);
			} else {
				_text.append(_line_buffer);
			}
			_text.insert_brush.clear_attributes();
			address += instruction.size();
		}
		_page_end = address;

	}

};

