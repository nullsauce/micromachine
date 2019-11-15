

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
		regs_text.width_policy.fixed(15);
		regs_text.height_policy.fixed(18);
		regs_text.border.enable();
	}

	bool timer_event() override {
		update();
		return Widget::timer_event();
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
		this->enable_animation(std::chrono::milliseconds(20));
		disasm_text.set_alignment(cppurses::Alignment::Left);
		disasm_text.clear();
		disasm_text.width_policy.minimum(20);
		disasm_text.width_policy.maximum(40);
		disasm_text.border.enable();
	}

	bool timer_event() override {
		update();
		return Widget::timer_event();
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

class Main_menu : public cppurses::layout::Horizontal {
public:
	cpu& _cpu;
	DisasmView& disasm_view;
	RegistersView& registers_view;
	Main_menu(cpu& cpu)
		: _cpu(cpu)
		, disasm_view(this->make_child<DisasmView>(_cpu))
		, registers_view(this->make_child<RegistersView>(_cpu))
	{
		this->focus_policy = cppurses::Focus_policy::Direct;
		this->set_name("Main_menu - head widget");
		key_pressed.connect([this](cppurses::Key::Code k){
			_cpu.step();
			update();
		});
	}

	void update() override  {
		disasm_view.update();
		registers_view.update();
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

	c.set_io_callback([](uint8_t op, uint8_t data){
		write(STDOUT_FILENO, &data, 1);
	});

	c.reset(program->entry_point());

	cppurses::System sys;
	Main_menu demo_menu(c);
	cppurses::System::set_initial_focus(&demo_menu);
	return sys.run(demo_menu);
}
