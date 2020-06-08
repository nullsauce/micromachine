#pragma once


#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/cppurses.hpp>

#include "BreakpointManager.hpp"
#include "ExecutionController.hpp"
#include "widgets/TopMenuView.hpp"
#include "widgets/DisasmView.hpp"
#include "widgets/BreakpointManagerView.hpp"
#include "widgets/RegistersView.hpp"
#include "widgets/MemoryBrowser.hpp"
#include "widgets/FoldableWidgetHeader.hpp"
#include "widgets/InterruptView.hpp"
#include "widgets/LogView.hpp"

class MainWindow : public cppurses::layout::Vertical {
private:
	mcu& _mcu;
	BreakpointManager _breakpoint_manager;
	ExecutionController _execution_controller;
	TopMenuView& _top_menu;
	cppurses::layout::Horizontal& _main_content_layout;
	cppurses::layout::Vertical& _first_col_layout;
	cppurses::layout::Vertical& _second_col_layout;
	cppurses::layout::Vertical& _third_col_layout;
	cppurses::layout::Vertical& _fourth_col_layout;
	DisasmView& _disasm_view;
	cppurses::Text_display& _help;
	RegistersView& _registers_view;
	BreakPointManagerView& _breakpoint_manager_view;
	MemoryBrowser& _memory_browser;
	InterruptView& _interrupt_view;
	LogView& _output_view;

public:
	MainWindow(mcu& mcu, uint32_t entry_point)
		: _mcu(mcu)
		, _execution_controller(_mcu, entry_point, _breakpoint_manager)
		, _top_menu(this->make_child<TopMenuView>())
		, _main_content_layout(this->make_child<cppurses::layout::Horizontal>())
		, _first_col_layout(_main_content_layout.make_child<cppurses::layout::Vertical>())
		, _second_col_layout(_main_content_layout.make_child<cppurses::layout::Vertical>())
		, _third_col_layout(_main_content_layout.make_child<cppurses::layout::Vertical>())
		, _fourth_col_layout(_main_content_layout.make_child<cppurses::layout::Vertical>())
		, _disasm_view(_first_col_layout.make_child<DisasmView>(_mcu, _breakpoint_manager, _execution_controller))
		, _help(_first_col_layout.make_child<cppurses::Text_display>())
		, _registers_view(_second_col_layout.make_child<RegistersView>(_mcu.get_cpu()))
		, _breakpoint_manager_view(_second_col_layout.make_child<BreakPointManagerView>(_breakpoint_manager))
		, _memory_browser(_third_col_layout.make_child<MemoryBrowser>(_mcu.get_memory()))
		, _interrupt_view(_third_col_layout.make_child<InterruptView>(_mcu))
		, _output_view(_fourth_col_layout.make_child<LogView>())
	{

		_top_menu.height_policy.fixed(1);

		_main_content_layout.focus_policy = cppurses::Focus_policy::None;
		_main_content_layout.height_policy.expanding(1000);
		_main_content_layout.border.disable();

		_first_col_layout.width_policy.fixed(41);
		_first_col_layout.border.enable();
		_first_col_layout.border.segments.disable_all();
		_first_col_layout.border.segments.east.enable();

		_second_col_layout.border.enable();
		_second_col_layout.border.segments.disable_all();
		_second_col_layout.border.segments.east.enable();
		_second_col_layout.width_policy.fixed(16);

		_third_col_layout.width_policy.fixed(59);
		_third_col_layout.border.enable();
		_third_col_layout.border.segments.disable_all();
		_third_col_layout.border.segments.east.enable();

		_help.height_policy.fixed(10);
		_help.border.enable();

		_disasm_view.focus_policy = cppurses::Focus_policy::Tab;
		_disasm_view.border.enable();
		_disasm_view.border.segments.disable_all();
		_disasm_view.border.segments.north.enable();

		_registers_view.focus_policy = cppurses::Focus_policy::None;
		_registers_view.height_policy.fixed(20);
		_registers_view.border.enable();
		_registers_view.border.segments.disable_all();
		_registers_view.border.segments.north.enable();

		_breakpoint_manager_view.focus_policy = cppurses::Focus_policy::Tab;
		_breakpoint_manager_view.border.enable();
		_breakpoint_manager_view.border.segments.disable_all();
		_breakpoint_manager_view.border.segments.north.enable();

		_memory_browser.focus_policy = cppurses::Focus_policy::Tab;
		_memory_browser.height_policy.max_size(25);
		_memory_browser.width_policy.fixed(58);
		_memory_browser.border.enable();
		_memory_browser.border.segments.disable_all();
		_memory_browser.border.segments.north.enable();

		_interrupt_view.focus_policy = cppurses::Focus_policy::None;
		_interrupt_view.height_policy.max_size(25);
		_interrupt_view.border.enable();
		_interrupt_view.border.segments.disable_all();
		_interrupt_view.border.segments.north.enable();


		_output_view.focus_policy = cppurses::Focus_policy::None;
		_output_view.border.enable();
		_output_view.border.segments.disable_all();
		_output_view.border.segments.north.enable();

		_mcu.set_io_callback([this](uint8_t data){
			_output_view.append_char((char)data);
		});

		_execution_controller.update_required.connect([this](){
			update();
		});

		enable_animation(std::chrono::milliseconds(10));

		_disasm_view.focused_in.connect([this]{ _help.set_contents(_disasm_view.help_content()); });
		_breakpoint_manager_view.focused_in.connect([this]{ _help.set_contents(_breakpoint_manager_view.help_content()); });
		_memory_browser.focused_in.connect([this]{ _help.set_contents(_memory_browser.help_content()); });

		focus_policy = cppurses::Focus_policy::None;

	}

	DisasmView& disasm_view() {
		return _disasm_view;
	}

	bool timer_event() override {
		static bool initial_update_done = false;

		if(!initial_update_done) {
			for(auto w : children.get_descendants()) {
				w->update();
			}
			initial_update_done = true;
		}

		_execution_controller.run_multi_steps();
		return Widget::timer_event();
	}

	void update() override {
		_main_content_layout.update();
		_disasm_view.update();
		_registers_view.update();
		_breakpoint_manager_view.update();
		_memory_browser.update();
		_interrupt_view.update();
		Widget::update();
	}
};


