

#include <chrono>
#include <unistd.h>
#include "cpu.hpp"
#include "programmer.hpp"

#include <cppurses/system/events/key.hpp>
#include <cppurses/system/shortcuts.hpp>
#include <cppurses/system/system.hpp>
#include <cppurses/widget/focus_policy.hpp>
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/menu.hpp>
#include <cppurses/system/focus.hpp>
#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/widgets/menu_stack.hpp>
#include <cppurses/widget/widgets/titlebar.hpp>
#include <cppurses/widget/widgets/text_display.hpp>
#include <queue>

namespace {
	void set_background(cppurses::Border& border, cppurses::Color color) {
		border.segments.north.brush.set_background(color);
		border.segments.south.brush.set_background(color);
		border.segments.east.brush.set_background(color);
		border.segments.west.brush.set_background(color);
		border.segments.north_east.brush.set_background(color);
		border.segments.north_west.brush.set_background(color);
		border.segments.south_east.brush.set_background(color);
		border.segments.south_west.brush.set_background(color);
	}

	void set_foreground(cppurses::Border& border, cppurses::Color color) {
		border.segments.north.brush.set_foreground(color);
		border.segments.south.brush.set_foreground(color);
		border.segments.east.brush.set_foreground(color);
		border.segments.west.brush.set_foreground(color);
		border.segments.north_east.brush.set_foreground(color);
		border.segments.north_west.brush.set_foreground(color);
		border.segments.south_east.brush.set_foreground(color);
		border.segments.south_west.brush.set_foreground(color);
	}
}

class RegistersView : public cppurses::layout::Vertical {
private:
	cpu& _cpu;
	uint32_t _previous_values[16];
	const char* const register_names[16] = {
		"R0 ","R1 ","R2 ","R3 ",
		"R4 ","R5 ","R6 ","R7 ",
		"R8 ","R9 ","R10","R11",
		"R12","SP ","LR ","PC "
	};

public:

	cppurses::Text_display& regs_text{this->make_child<cppurses::Text_display>()};

	RegistersView(cpu& cpu)
		: _cpu(cpu) {
		memset(_previous_values, 0, 16);
		regs_text.set_alignment(cppurses::Alignment::Left);
		regs_text.clear();
		width_policy.fixed(15);
		height_policy.fixed(18);
		border.enable();
	}

	bool paint_event() override {
		render();
		return Widget::paint_event();
	}

	void render() {
		regs_text.clear();
		for(int i = 0; i < 16; i++) {
			char line[64] = {0};
			uint32_t actual_reg_value = _cpu.regs().get(i);
			uint32_t previous_reg_value = _previous_values[i];
			bool changed = previous_reg_value != actual_reg_value;
			_previous_values[i] = actual_reg_value;
			sprintf(line, "%s %08x\n", register_names[i], actual_reg_value);
			if(changed) {
				regs_text.insert_brush.add_attributes(cppurses::Attribute::Standout);
				regs_text.append(line);
				regs_text.insert_brush.remove_attributes(cppurses::Attribute::Standout);
			} else {
				regs_text.append(line);
			}
		}
	}

	bool focus_in_event() override {
		set_background(border, cppurses::Color::Dark_red);
		set_foreground(border, cppurses::Color::Yellow);
		this->update();
		return Widget::focus_in_event();
	}

	bool focus_out_event() override {
		set_background(border, cppurses::Color::Black);
		set_foreground(border, cppurses::Color::White);
		this->update();
		return Widget::focus_out_event();
	}
};

class DisasmView : public cppurses::layout::Vertical {
private:
	cpu& _cpu;
	uint32_t _page_address;
	uint32_t _page_end;
public:
	cppurses::Text_display& disasm_text{this->make_child<cppurses::Text_display>()};

	DisasmView(cpu& cpu)
		: _cpu(cpu)
		, _page_address(0)
		, _page_end(0) {

		disasm_text.set_alignment(cppurses::Alignment::Left);
		disasm_text.clear();
		width_policy.minimum(22);
		width_policy.maximum(42);
		border.enable();
	}

	bool focus_in_event() override {
		set_background(border, cppurses::Color::Dark_red);
		set_foreground(border, cppurses::Color::Yellow);
		this->update();
		return Widget::focus_in_event();
	}

	bool focus_out_event() override {
		set_background(border, cppurses::Color::Black);
		set_foreground(border, cppurses::Color::White);
		this->update();
		return Widget::focus_out_event();
	}

	bool paint_event() override {
		render();
		return Widget::paint_event();
	}

	void render() {
		disasm_text.clear();
		uint32_t pc = _cpu.regs().get_pc();
		uint32_t address = pc;
		uint32_t lines_per_page = disasm_text.height();
		if(address > _page_end || address < _page_address) {
			_page_address = address;
		} else {
			address = _page_address;
		}
		char line[128] = {0};
		for(size_t i = 0; i < lines_per_page; i++) {
			auto instruction = _cpu.fetch_instruction(address);
			std::string disasm = disasm::disassemble_instruction(instruction, address);
			sprintf(line, "%08x│ %s\n", address, disasm.c_str());
			if(address == pc) {
				disasm_text.insert_brush.add_attributes(cppurses::Attribute::Standout);
				disasm_text.append(line);
				disasm_text.insert_brush.remove_attributes(cppurses::Attribute::Standout);
			} else {
				disasm_text.append(line);
			}
			address += instruction.size();
		}
		_page_end = address;
	}

};

class LogView : public cppurses::layout::Vertical  {
private:
	std::deque<std::string> _lines;

public:
	cppurses::Text_display& _text{this->make_child<cppurses::Text_display>()};

	bool paint_event() override {
		render();
		return Widget::paint_event();
	}

	void append_line(const std::string& txt) {
		_lines.push_back(txt + "\n");
		update();
	}

	void render() {
		_text.clear();
		size_t displayable_lines = _text.height();
		auto start = std::prev(_lines.end(), std::min(displayable_lines, _lines.size()));
		while(start != _lines.end()) {
			_text.append(*start);
			start++;
		}
	}

	bool focus_in_event() override {
		set_background(border, cppurses::Color::Dark_red);
		set_foreground(border, cppurses::Color::Yellow);
		this->update();
		return Widget::focus_in_event();
	}

	bool focus_out_event() override {
		set_background(border, cppurses::Color::Black);
		set_foreground(border, cppurses::Color::White);
		this->update();
		return Widget::focus_out_event();
	}
};



class Main_menu : public cppurses::layout::Horizontal {
private:
	cpu& _cpu;
	DisasmView& _disasm_view;
	RegistersView& registers_view;
	LogView& log_view;
	Widget& box;

public:
	Main_menu(cpu& cpu)
		: _cpu(cpu)
		, _disasm_view(this->make_child<DisasmView>(_cpu))
		, registers_view(this->make_child<RegistersView>(_cpu))
		, log_view(this->make_child<LogView>())
		, box(this->make_child<Widget>())
	{
		log_view.width_policy.fixed(30);
		log_view.height_policy.expanding(10);
		log_view.border.enable();
		focus_policy = cppurses::Focus_policy::Tab;
		box.border.enable();
		box.width_policy.fixed(20);
		box.height_policy.fixed(4);

		_disasm_view.focus_policy = cppurses::Focus_policy::Tab;
		registers_view.focus_policy = cppurses::Focus_policy::Tab;
		log_view.focus_policy = cppurses::Focus_policy::Tab;

		set_name("Main_menu - head widget");
		enable_animation(std::chrono::milliseconds(10));



		/*
		key_pressed.connect([this](cppurses::Key::Code k){
			for(int i = 0; i < 1; i++) {
				_cpu.step();
			}
			//log_view.append_line("Hey" + std::to_string(_cpu.regs().get_pc()));
			update();
		});*/

		_cpu.set_io_callback([this](uint8_t op, uint8_t data){
			static std::stringstream linebuf;
			linebuf.put(data);
			if(data == '\n') {
				log_view.append_line(linebuf.str());
				linebuf.clear();
			}
		});


	}

	bool timer_event() override {
		_cpu.step();
		update();
		return Widget::timer_event();
	}

	void update() override  {
		_disasm_view.update();
		registers_view.update();
		log_view.update();
		Widget::update();
	}
};

int main(int argc, const char** argv) {

	if(argc < 2) {
		fprintf(stderr, "usage: %s <elf-executable>\n", argv[0]);
		return EXIT_FAILURE;
	}

	cpu c;
	programmer::program::ptr program = programmer::load_elf(argv[1], c.mem());

	if(program->is_null()) {
		fprintf(stderr, "Error: Failed to load the given ELF file %s\n", argv[0]);
		return EXIT_FAILURE;
	}


	c.reset(program->entry_point());

	cppurses::System sys;
	Main_menu demo_menu(c);
	cppurses::System::set_initial_focus(&demo_menu);
	return sys.run(demo_menu);
}
