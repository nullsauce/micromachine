

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
#include <cppurses/widget/widgets/textbox.hpp>
#include <regex>

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


class SingleLineTextInput : public cppurses::Textbox {
public:
	bool _placeholder_is_set;
	sig::Signal<void(const cppurses::Glyph_string&)> text_entered;
	cppurses::Glyph_string _placeholder_text;
	cppurses::Color _saved_color;

	SingleLineTextInput(const cppurses::Glyph_string& placeholder)
		: _placeholder_is_set(true)
		, _placeholder_text(placeholder) {
		enable_placeholder();
		height_policy.fixed(3);
		border.enable();

	}

	bool key_press_event(const cppurses::Key::State& keyboard) override {
		if(keyboard.key == cppurses::Key::Enter) {
			text_entered(this->contents());
			return false;
		} else if(contents().empty()) {
				enable_placeholder();
				return false;
		} else {
			if(_placeholder_is_set) {
				disable_placeholder();
			}
			return Textbox::key_press_event(keyboard);
		}
	}

	void enable_placeholder() {
		_saved_color = brush.foreground_color().get();
		brush.set_foreground(cppurses::Color::Dark_gray);
		set_contents(_placeholder_text);
		_placeholder_is_set = true;
	}

	void disable_placeholder() {
		brush.set_foreground(cppurses::Color::White);
		clear();
		_placeholder_is_set = false;
	}
};

template<class ViewT>
class CommandableView : public cppurses::layout::Stack  {
protected:
	SingleLineTextInput& _command_input;
	ViewT& _view;
public:
	template<typename... Args>
	CommandableView(Args&&... args)
		: _command_input(make_page<SingleLineTextInput>("command..."))
		, _view(make_page<ViewT>(std::forward<Args>(args)...)) {

		_command_input.height_policy.fixed(3);
		_command_input.text_entered.connect([this](const cppurses::Glyph_string& text){
			if(process_command(text.str())) {
				close_menu();
			}
		});
		_view.border.disable();
		key_pressed.connect([this](cppurses::Key::Code key){
			if(key == cppurses::Key::o) {
				open_menu();
			}
		});
		set_active_page(1);
	}

	void open_menu() {
		set_active_page(0);
		cppurses::Focus::set_focus_to(_command_input);
	}

	void close_menu() {
		set_active_page(1);
		cppurses::Focus::set_focus_to(*this);
	}

private:

	bool process_command(const std::string& text) {
		for(const command& c : _commands) {
			std::smatch match;
			if(std::regex_match(text, match, c.pattern)) {
				std::vector<std::string> groups;
				for(size_t i = 0; i < match.size(); i++) {
					groups.push_back(match[i]);
				}
				return handle_command(groups);
			}
		}
		return false;
	}

protected:
	struct command {
		const std::string name;
		const std::string description;
		const std::regex pattern;
	};

	std::vector<command> _commands;

	void register_command(const std::string& name, const std::regex& pattern, const std::string& description = "") {
		_commands.push_back({name, description, pattern});
	}

	virtual bool handle_command(const std::vector<std::string>& args) {
		return false;
	}
};

class MemoryView : public cppurses::layout::Vertical {
private:
	const memory& _memory;
	uint32_t _address;
	cppurses::Text_display& _text;

public:
	MemoryView(memory& memory)
		: _memory(memory)
		, _address(0)
		, _text(make_child<cppurses::Text_display>()) {
	}

	bool paint_event() override {
		render();
		return Widget::paint_event();
	}

	void render() {
		_text.clear();
		size_t displayable_lines = _text.height();
		size_t displayable_columns = (_text.width()-9) / 3;
		size_t displayable_words = displayable_columns / 4;
		static std::vector<char> buff;
		buff.reserve(16 + (displayable_words * 4 * 3));
		std::stringstream ss;
		for(size_t l = 0; l < displayable_lines; l++) {
			const uint32_t line_address = _address + (l * displayable_columns);
			std::sprintf(buff.data(), "%08x|", (uint32_t)line_address);
			ss << buff.data();
			for(size_t w = 0; w < displayable_words; w++) {
				std::sprintf(buff.data(), "%02x %02x %02x %02x ",
					_memory.read8(line_address + 0),
					_memory.read8(line_address + 1),
					_memory.read8(line_address + 2),
					_memory.read8(line_address + 3)
				);
				ss << buff.data();
			}
			ss << std::endl;
		}
		_text.set_contents(ss.str());

	}
};


class SomeView : public CommandableView<MemoryView> {
private:

public:

	SomeView(cpu& cpu)
	 : CommandableView<MemoryView>(cpu.mem()) {

		register_command("goto", std::regex("(g(?:oto))? ([a-f0-9]+)"));

	}

	bool handle_command(const std::vector<std::string>& args) override {
		if(args[1] == "goto" || args[1] == "g") {
			return true;
		}
		return false;
	}

protected:
	virtual void handle_command(const std::string& cmd) {

	}
};

class Main_menu : public cppurses::layout::Horizontal {
private:
	cpu& _cpu;
	DisasmView& _disasm_view;
	RegistersView& registers_view;
	SomeView& log_view;

public:
	Main_menu(cpu& cpu)
		: _cpu(cpu)
		, _disasm_view(this->make_child<DisasmView>(_cpu))
		, registers_view(this->make_child<RegistersView>(_cpu))
		, log_view(this->make_child<SomeView>(cpu))
	{
		log_view.width_policy.expanding(10);
		log_view.width_policy.min_size(14);
		log_view.height_policy.expanding(10);
		log_view.border.enable();
		focus_policy = cppurses::Focus_policy::Tab;

		_disasm_view.focus_policy = cppurses::Focus_policy::Tab;
		registers_view.focus_policy = cppurses::Focus_policy::Tab;
		log_view.focus_policy = cppurses::Focus_policy::Tab;

		set_name("Main_menu - head widget");
		enable_animation(std::chrono::milliseconds(10));

		_cpu.set_io_callback([this](uint8_t op, uint8_t data){
			static std::stringstream linebuf;
			linebuf.put(data);
			if(data == '\n') {
				//log_view.append_line(linebuf.str());
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
