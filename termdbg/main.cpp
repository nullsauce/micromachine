

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

	template<typename TO, typename FROM>
	std::unique_ptr<TO> static_unique_pointer_cast (std::unique_ptr<FROM>&& old){
		return std::unique_ptr<TO>{static_cast<TO*>(old.release())};
		//conversion: unique_ptr<FROM>->FROM*->TO*->unique_ptr<TO>
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
		set_foreground(border, cppurses::Color::Yellow);
		this->update();
		return Widget::focus_in_event();
	}

	bool focus_out_event() override {
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
		set_foreground(border, cppurses::Color::Yellow);
		this->update();
		return Widget::focus_in_event();
	}

	bool focus_out_event() override {
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
		set_foreground(border, cppurses::Color::Yellow);
		this->update();
		return Widget::focus_in_event();
	}

	bool focus_out_event() override {
		set_foreground(border, cppurses::Color::White);
		this->update();
		return Widget::focus_out_event();
	}
};


class SingleLineTextInput : public cppurses::Textbox {
public:
	bool _placeholder_is_set;
	sig::Signal<void(const cppurses::Glyph_string&)> text_entered;
	sig::Signal<void()> input_aborted;
	cppurses::Glyph_string _placeholder_text;

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
		} else if(keyboard.key == cppurses::Key::Escape) {
			input_aborted();
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
		brush.set_foreground(cppurses::Color::Dark_gray);
		set_contents(_placeholder_text);
		_placeholder_is_set = true;
	}

	void disable_placeholder() {
		brush.set_foreground(cppurses::Color::White);
		clear();
		_placeholder_is_set = false;
	}

	bool focus_in_event() override {
		set_foreground(border, cppurses::Color::Yellow);
		this->update();
		return Widget::focus_in_event();
	}

	bool focus_out_event() override {
		set_foreground(border, cppurses::Color::White);
		this->update();
		return Widget::focus_out_event();
	}
};

template<class ViewT>
class CommandableView : public cppurses::layout::Vertical  {
protected:
	std::unique_ptr<SingleLineTextInput> _offscreen_input;
	ViewT& _view;
public:
	template<typename... Args>
	CommandableView(Args&&... args)
		: _offscreen_input(std::make_unique<SingleLineTextInput>("command..."))
		, _view(make_child<ViewT>(std::forward<Args>(args)...)) {
		_offscreen_input->set_name("command_input");
		_offscreen_input->height_policy.fixed(3);
		_offscreen_input->text_entered.connect([this](const cppurses::Glyph_string& text){
			if(process_command(text.str())) {
				close_menu();
			}
		});
		_offscreen_input->input_aborted.connect([this](){
			close_menu();
		});
		_view.border.disable();
		key_pressed.connect([this](cppurses::Key::Code key){
			if(key == cppurses::Key::o) {
				open_menu();
			}
		});
		//set_active_page(1);

	}

	void open_menu() {
		//set_active_page(0);
		//_command_input.enable(true, true);
		if(_offscreen_input) {
			children.insert(std::move(_offscreen_input), 0);
			cppurses::Focus::set_focus_to(*children.get()[0]);
		}
		update();
	}

	void close_menu() {
		//set_active_page(1);
		//_command_input.disable(true, true);
		_offscreen_input = static_unique_pointer_cast<SingleLineTextInput, cppurses::Widget>(children.remove("command_input"));
		cppurses::Focus::set_focus_to(*this);
		update();
	}

	bool focus_in_event() override {
		set_foreground(border, cppurses::Color::Yellow);
		this->update();
		return Widget::focus_in_event();
	}

	bool focus_out_event() override {
		set_foreground(border, cppurses::Color::White);
		this->update();
		return Widget::focus_out_event();
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
	uint32_t _cursor_x;
	uint32_t _cursor_y;
	cppurses::Text_display& _text;

public:
	MemoryView(memory& memory)
		: _memory(memory)
		, _address(0)
		, _cursor_x(0)
		, _cursor_y(0)
		, _text(make_child<cppurses::Text_display>()) {

	}

	bool paint_event() override {
		render();
		update();
		return Widget::paint_event();
	}

	void scroll_byte_up() {
		scroll_up(1);
	}

	void scroll_byte_down() {
		scroll_down(1);
	}

	void scroll_line_up() {
		scroll_up(displayable_clomuns());
	}
	void scroll_line_down() {
		scroll_down(displayable_clomuns());
	}

	void scroll_page_up() {
		scroll_up(displayable_bytes());
	}
	void scroll_page_down() {
		scroll_down(displayable_bytes());
	}

	void scroll_up(uint32_t offset) {
		set_address(_address + offset);
	}

	void scroll_down(uint32_t offset) {
		set_address(_address - offset);
	}

	void set_address(uint32_t address) {
		_address = address;
		update();
	}

	void navigate_to_address(uint32_t address) {
		_cursor_x = 0;
		_cursor_y = 0;
		set_address(address);
	}

	void move_cursor_left() {
		if(_cursor_x == 0) {
			_cursor_x = displayable_clomuns() - 1;
		} else {
			_cursor_x--;
		}
		update();
	}

	void move_cursor_right() {
		if(_cursor_x < displayable_clomuns() - 1) {
			_cursor_x++;
		} else {
			_cursor_x = 0;
		}
		update();
	}

	void move_cursor_up() {
		if(_cursor_y > 0) {
			_cursor_y--;
		} else {
			scroll_line_down();
			_cursor_y = 0;
		}
		update();
	}

	void move_cursor_down() {
		if(_cursor_y < displayable_lines() - 1) {
			_cursor_y++;
		} else {
			scroll_line_up();
			_cursor_y = displayable_lines() - 1;
		}
		update();
	}

	uint32_t cursor_address() const {
		return _address + (_cursor_y * displayable_clomuns()) + (_cursor_x);

	}

	size_t displayable_lines() const {
		return _text.height();
	}

	size_t max_displayable_clomuns() const {
		return std::max(0,((int)_text.width()-10)) / 3;
	}

	size_t displayable_clomuns() const {
		return (max_displayable_clomuns() / 4) * 4;
	}

	size_t displayable_bytes() const {
		return displayable_clomuns() * displayable_lines();
	}

	void render() {

		_text.clear();
		static std::vector<char> buff;
		buff.reserve(16 + (displayable_clomuns() * 3));
		std::stringstream ss;
		for(size_t l = 0; l < displayable_lines(); l++) {
			const uint32_t line_address = _address + (l * displayable_clomuns());
			std::sprintf(buff.data(), "%08x│", (uint32_t)line_address);
			_text.append(buff.data());
			for(size_t c = 0; c < displayable_clomuns(); c++) {
				uint32_t address = line_address + c;
				std::sprintf(buff.data(), "%02x ", _memory.read8_unchecked(address));
				if(cursor_address() == address) {
					_text.insert_brush.add_attributes(cppurses::Attribute::Standout);
					_text.append(buff.data());
					_text.insert_brush.remove_attributes(cppurses::Attribute::Standout);
				} else {
					_text.append(buff.data());
				}
			}
		}
		_text.append("\n");
		//this->cursor.set_position(cppurses::Point{20,1});
	}
};


class SomeView : public CommandableView<MemoryView> {

public:

	SomeView(cpu& cpu)
	 : CommandableView<MemoryView>(cpu.mem()) {
		register_command("goto", std::regex("(g(?:oto))? ([a-f0-9]+)"));
	}

	bool key_press_event(const cppurses::Key::State& keyboard) override {
		auto key = keyboard.key;
		if(key == cppurses::Key::Code::Arrow_up) {
			//_view.scroll_line_down();
			_view.move_cursor_up();
			return true;
		} else if(key == cppurses::Key::Code::Arrow_down) {
			//_view.scroll_line_up();
			_view.move_cursor_down();
			return true;
		} else if(key == cppurses::Key::Code::Arrow_left) {
			//_view.scroll_line_up();
			_view.move_cursor_left();
			return true;
		} else if(key == cppurses::Key::Code::Arrow_right) {
			//_view.scroll_line_up();
			_view.move_cursor_right();
			return true;
		} else if(key == 337) {
			//_view.scroll_page_down();
			return true;
		} else if(key == 336) {
			//_view.scroll_page_up();
			return true;
		}
		update();
		return CommandableView<MemoryView>::key_press_event(keyboard);
	}


	bool handle_command(const std::vector<std::string>& args) override {
		if(args[1] == "goto" || args[1] == "g") {
			uint32_t address = 0;
			if(1 == sscanf(args[2].data(),"%x", &address)) {
				_view.navigate_to_address(address);
				return true;
			}
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
		log_view.width_policy.min_size(20);
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
