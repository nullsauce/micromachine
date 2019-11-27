

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
#include <cppurses/widget/widgets/push_button.hpp>
#include <cppurses/widget/widgets/label.hpp>

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


class CommandInput : public SingleLineTextInput {
public:
	sig::Signal<void(const std::string&, const std::vector<std::string>&)> command_parsed;
	CommandInput(const cppurses::Glyph_string& placeholder)
		: SingleLineTextInput(placeholder){
		text_entered.connect(std::bind(&CommandInput::handle_command, this, std::placeholders::_1));
	}

	void register_command(const std::string& command, const std::regex& pattern, const std::string& description = "") {
		_commands.push_back({command, description, pattern});
	}

private:
	struct command {
		const std::string name;
		const std::string description;
		const std::regex pattern;
	};

	std::vector<command> _commands;

	bool handle_command(const cppurses::Glyph_string& text) {
		for(const command& c : _commands) {
			std::smatch match;
			std::string input = text.str();
			if(std::regex_match(input, match, c.pattern)) {
				if(match.size() < 2) continue;
				std::vector<std::string> groups;
				for(size_t i = 2; i < match.size(); i++) {
					groups.push_back(match[i]);
				}
				command_parsed(match[1], groups);
			}
		}
		return false;
	}
};

template<class Widg_t>
class HidableElement {
private:
	std::unique_ptr<Widg_t> _menu_widget;
	cppurses::Widget* _parent;
public:

	template<typename... Args>
	HidableElement(cppurses::Widget* parent, Args&&... args)
		: _menu_widget(std::make_unique<Widg_t>(std::forward<Args>(args)...))
		, _parent(parent)
	{}

	void show() {
		if(_menu_widget) {
			_parent->children.insert(std::move(_menu_widget), 0);
			cppurses::Focus::set_focus_to(*_parent->children.get()[0].get());
		}
	}

	void hide() {
		if(_parent->children.get().size() > 0) {
			cppurses::Widget* widget = _parent->children.get()[0].get();
			_menu_widget = static_unique_pointer_cast<Widg_t, cppurses::Widget>(_parent->children.remove(widget));
			cppurses::Focus::set_focus_to(*_parent);
		}
	}

	Widg_t& get() {
		return *_menu_widget.get();
	}
};

class MemoryView : public cppurses::layout::Vertical {
protected:
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
	}
};

class Table: public cppurses::layout::Vertical {
public:
	Table(int columns)
		: _num_columns(columns)
		, _num_cells(0) {
		border.enable();
		border.segments.south.disable();
		border.segments.west.disable();
		border.segments.east.disable();
		border.segments.south_west.disable();
		border.segments.south_east.disable();
	}

	template <typename Widg_t, typename... Args>
	Widg_t& make_cell(Args&&... args) {
		auto& row = get_next_free_row();
		auto& cell = row.make_child<Widg_t>(std::forward<Args>(args)...);
		cell.border.enable();
		cell.border.segments.west.disable();
		cell.border.segments.north.disable();
		cell.border.segments.north_west.disable();
		cell.border.segments.north_east.disable();
		cell.border.segments.south_west.disable();
		_num_cells++;
		return cell;
	}

	bool resize_event(cppurses::Area new_size, cppurses::Area old_size) override {
		// prevent rows with empty cells to use all the horizontal space by
		// forcing the width on each cell.
		bool res = cppurses::layout::Vertical::resize_event(new_size, old_size);
		for(auto& row : _rows) {
			for(auto& cell : row.get().children.get()) {
				cell.get()->width_policy.fixed(std::max(0, (int)(width() / _num_columns)));
			}
		}
		return res;
	}

	template <typename Widg_t>
	Widg_t& get_cell_at(size_t col, size_t row) {
		return static_cast<Widg_t&>(*_rows[row].get().children.get().at(col).get());
	}

	template <typename Widg_t>
	Widg_t& get_cell_at(size_t index) {
		return get_cell_at<Widg_t>(index % _num_columns, index / _num_columns);
	}

	size_t num_rows() const {
		return _rows.size();
	}

private:
	const size_t _num_columns;
	size_t _num_cells;
	std::vector<std::reference_wrapper<cppurses::layout::Horizontal>> _rows;

	cppurses::layout::Horizontal& get_next_free_row() {
		if(_num_cells % _num_columns == 0) {
			return make_empty_row();
		} else {
			return _rows.at(_rows.size()-1).get();
		}
	}

	cppurses::layout::Horizontal& make_empty_row() {
		auto& row = make_child<cppurses::layout::Horizontal>();
		_rows.push_back(row);
		return row;
	}
};

class RightAlignedText : public cppurses::Text_display {
public:
	RightAlignedText(const cppurses::Glyph_string& text) : cppurses::Text_display(text) {
		set_alignment(cppurses::Alignment::Right);
	}
};


class SomeView : public cppurses::layout::Vertical {
public:
	cpu& _cpu;
	MemoryView& _memview;
	HidableElement<CommandInput> _menu;
	Table& _info_table {make_child<Table>(4)};

	SomeView(cpu& cpu)
	 : _cpu(cpu)
	 , _memview(make_child<MemoryView>(cpu.mem()))
	 , _menu(this, cppurses::Glyph_string("command...")) {
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


		_menu.get().register_command("goto", std::regex("(goto) ([a-f0-9]+)"));
		_menu.get().command_parsed.connect([this](const std::string& command, const std::vector<std::string>& args){
			if(process_command(command, args)) {
				close_menu();
			}
		});

		_menu.get().input_aborted.connect([this](){
			close_menu();
		});

		key_pressed.connect([this](cppurses::Key::Code key){
			if(key == cppurses::Key::o) {
				open_menu();
			}
		});
	}

	void close_menu() {
		_menu.hide();
	}

	void open_menu() {
		_menu.show();
	}

	bool process_command(const std::string& command, const std::vector<std::string>& args) {
		if(command == "goto" && args.size() > 0) {
			uint32_t address = 0;
			if(1 == sscanf(args[0].data(), "%x", &address)) {
				_memview.navigate_to_address(address);
				return true;
			}
		}
		return true;
	}

	bool paint_event() override {
		render();
		update();
		return Widget::paint_event();
	}

	void render() {
		char buff[32];
		snprintf(buff, 32, "%08x", _memview.cursor_address());
		_info_table.get_cell_at<RightAlignedText>(0,0).set_contents(buff);
		_info_table.get_cell_at<RightAlignedText>(1,1).set_contents(std::to_string(
			(uint8_t)_cpu.mem().read8_unchecked(_memview.cursor_address())
		));
		_info_table.get_cell_at<RightAlignedText>(2,1).set_contents(std::to_string(
			(uint16_t)_cpu.mem().read16_unchecked(_memview.cursor_address())
		));
		_info_table.get_cell_at<RightAlignedText>(3,1).set_contents(std::to_string(
			(uint32_t)_cpu.mem().read32_unchecked(_memview.cursor_address())
		));

		_info_table.get_cell_at<RightAlignedText>(1,2).set_contents(std::to_string(
			(int8_t)_cpu.mem().read8_unchecked(_memview.cursor_address())
		));
		_info_table.get_cell_at<RightAlignedText>(2,2).set_contents(std::to_string(
			(int16_t)_cpu.mem().read16_unchecked(_memview.cursor_address())
		));
		_info_table.get_cell_at<RightAlignedText>(3,2).set_contents(std::to_string(
			(int32_t)_cpu.mem().read32_unchecked(_memview.cursor_address())
		));

		_info_table.get_cell_at<RightAlignedText>(1,3).set_contents(std::to_string(
			(uint8_t)_cpu.mem().read8_unchecked(_memview.cursor_address())
		));
		_info_table.get_cell_at<RightAlignedText>(2,3).set_contents(std::to_string(
			(uint16_t)__builtin_bswap16(_cpu.mem().read16_unchecked(_memview.cursor_address()))
		));
		_info_table.get_cell_at<RightAlignedText>(3,3).set_contents(std::to_string(
			(uint32_t)__builtin_bswap32(_cpu.mem().read32_unchecked(_memview.cursor_address()))
		));

		_info_table.get_cell_at<RightAlignedText>(1,4).set_contents(std::to_string(
			(int8_t)_cpu.mem().read8_unchecked(_memview.cursor_address())
		));
		_info_table.get_cell_at<RightAlignedText>(2,4).set_contents(std::to_string(
			(int16_t)__builtin_bswap16(_cpu.mem().read16_unchecked(_memview.cursor_address()))
		));
		_info_table.get_cell_at<RightAlignedText>(3,4).set_contents(std::to_string(
			(int32_t)__builtin_bswap32(_cpu.mem().read32_unchecked(_memview.cursor_address()))
		));
	}

	bool key_press_event(const cppurses::Key::State& keyboard) override {
		auto key = keyboard.key;
		if(key == cppurses::Key::Code::o) {
			_menu.show();
			return true;
		} else if(key == cppurses::Key::Code::Arrow_up) {
			//_view.scroll_line_down();
			_memview.move_cursor_up();
			return true;
		} else if(key == cppurses::Key::Code::Arrow_down) {
			//_view.scroll_line_up();
			_memview.move_cursor_down();
			return true;
		} else if(key == cppurses::Key::Code::Arrow_left) {
			//_view.scroll_line_up();
			_memview.move_cursor_left();
			return true;
		} else if(key == cppurses::Key::Code::Arrow_right) {
			//_view.scroll_line_up();
			_memview.move_cursor_right();
			return true;
		} else if(key == 337) {
			//_view.scroll_page_down();
			return true;
		} else if(key == 336) {
			//_view.scroll_page_up();
			return true;
		}
		update();
		return _memview.key_press_event(keyboard);
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

class ExecControlView : public cppurses::layout::Vertical {
private:
	cppurses::Push_button& _play_btn {make_child<cppurses::Push_button>(">")};
	cppurses::Push_button& _pause_btn {make_child<cppurses::Push_button>("||")};
	cppurses::Push_button& _step_btn {make_child<cppurses::Push_button>(">|")};

public:
	ExecControlView() {
		_play_btn.clicked.connect([this](){
			_play_btn.border.disable();
		});
		_play_btn.border.enable();
		_play_btn.border.segments.south.disable();
		_play_btn.border.segments.south_east.disable();
		_play_btn.border.segments.south_west.disable();
		_pause_btn.border.enable();
		_pause_btn.border.segments.south_west.symbol = L'├';
		_pause_btn.border.segments.south_east.symbol = L'┤';
		_pause_btn.border.segments.north_west.symbol = L'├';
		_pause_btn.border.segments.north_east.symbol = L'┤';

		_step_btn.border.enable();
		_play_btn.height_policy.fixed(2);
		_pause_btn.height_policy.fixed(3);

		_step_btn.border.segments.north.disable();
		_step_btn.border.segments.north_east.disable();
		_step_btn.border.segments.north_west.disable();
		_step_btn.height_policy.fixed(2);
		//border.enable();
	}

};

class Main_menu : public cppurses::layout::Horizontal {
private:
	cpu& _cpu;
	DisasmView& _disasm_view;
	cppurses::layout::Vertical& _central_panel;
	RegistersView& registers_view;
	ExecControlView& _exec_control_view;
	SomeView& log_view;

public:
	Main_menu(cpu& cpu)
		: _cpu(cpu)
		, _disasm_view(this->make_child<DisasmView>(_cpu))
		, _central_panel(this->make_child<cppurses::layout::Vertical>())
		, registers_view(_central_panel.make_child<RegistersView>(_cpu))
		, _exec_control_view(_central_panel.make_child<ExecControlView>())
		, log_view(this->make_child<SomeView>(cpu))
	{

		//_central_panel.width_policy.minimum(22);
		_central_panel.width_policy.maximum(15);
		log_view.width_policy.expanding(10);
		log_view.width_policy.min_size(24);

		log_view.border.enable();

		focus_policy = cppurses::Focus_policy::Tab;

		_central_panel.focus_policy = cppurses::Focus_policy::Tab;
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
	demo_menu.width_policy.min_size(100);
	cppurses::System::set_initial_focus(&demo_menu);
	return sys.run(demo_menu);
}
