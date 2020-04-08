/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_MAINWINDOW_HPP
#define MICROMACHINE_EMU_MAINWINDOW_HPP


#include <cppurses/widget/layouts/vertical.hpp>

#include "BreakpointManager.hpp"
#include "widgets/TopMenuView.hpp"
#include "widgets/DisasmView.hpp"
#include "widgets/BreakpointManagerView.hpp"
#include "widgets/RegistersView.hpp"

class MainWindow : public cppurses::layout::Vertical {
private:
	cpu& _cpu;
	BreakpointManager _breakpoint_manager;
	TopMenuView& _top_menu;
	cppurses::layout::Horizontal& _main_content_layout;
	cppurses::layout::Vertical& _left_col_layout;
	cppurses::layout::Vertical& _mid_col_layout;
	cppurses::layout::Vertical& _right_col_layout;
	DisasmView& _disasm_view;
	BreakPointManagerView& _breakpoint_manager_view;
	RegistersView& _registers_view;
	//MemoryBrowser& _memory_browser;
	bool _process_steps;


public:
	MainWindow(cpu& cpu)
		: _cpu(cpu)
		, _top_menu (this->make_child<TopMenuView>())
		, _main_content_layout(this->make_child<cppurses::layout::Horizontal>())
		, _left_col_layout(_main_content_layout.make_child<cppurses::layout::Vertical>())
		, _mid_col_layout(_main_content_layout.make_child<cppurses::layout::Vertical>())
		, _right_col_layout(_main_content_layout.make_child<cppurses::layout::Vertical>())
		, _disasm_view(_left_col_layout.make_child<DisasmView>(_cpu, _breakpoint_manager))
		, _breakpoint_manager_view(_mid_col_layout.make_child<BreakPointManagerView>(_breakpoint_manager))
		, _registers_view(_mid_col_layout.make_child<RegistersView>(_cpu))
		//, _memory_browser(this->make_child<MemoryBrowser>(cpu))
		, _process_steps(false)
	{

		focus_policy = cppurses::Focus_policy::Tab;

		_top_menu.height_policy.fixed(1);

		_main_content_layout.focus_policy = cppurses::Focus_policy::None;
		_main_content_layout.height_policy.expanding(1000);
		_main_content_layout.border.disable();


		//_left_col_layout.border.enable();
		//_mid_col_layout.border.enable();
		//_right_col_layout.border.enable();

		_disasm_view.focus_policy = cppurses::Focus_policy::Tab;
		//_disasm_view.width_policy.fixed(50);
		_disasm_view.border.enable();
		_disasm_view.border.segments.disable_all();
		_disasm_view.border.segments.east.enable();
		_disasm_view.border.segments.north_east.enable();
		_disasm_view.border.segments.north.enable();

		_breakpoint_manager_view.focus_policy = cppurses::Focus_policy::Tab;
		//_breakpoint_manager_view.width_policy.fixed(25);
		_breakpoint_manager_view.height_policy.fixed(8);
		_breakpoint_manager_view.border.enable();
		_breakpoint_manager_view.border.segments.disable_all();
		_breakpoint_manager_view.border.segments.east.enable();
		_breakpoint_manager_view.border.segments.north_east.enable();
		_breakpoint_manager_view.border.segments.north.enable();

		_registers_view.focus_policy = cppurses::Focus_policy::Tab;
		//_registers_view.width_policy.fixed(15);
		_registers_view.height_policy.fixed(18);
		_registers_view.border.enable();
		_registers_view.border.segments.disable_all();
		_registers_view.border.segments.east.enable();
		_registers_view.border.segments.north_east.enable();
		_registers_view.border.segments.north.enable();

		_cpu.set_io_callback([this](uint8_t op, uint8_t data){
			static std::stringstream linebuf;
			linebuf.put(data);
			if(data == '\n') {
				//_memory_browser.append_line(linebuf.str());
				linebuf.clear();
			}
		});

		enable_animation(std::chrono::milliseconds(10));
	}

	bool key_press_event(const cppurses::Key::State& keyboard) override {
		if(paused() && keyboard.key == cppurses::Key::s) {
			step();
			return true;
		} else if(paused() && keyboard.key == cppurses::Key::r) {
			resume();
			return true;
		} else if(!paused() && keyboard.key == cppurses::Key::p) {
			pause();
			return true;
		}
		return false;
	}

	void resume() {
		_process_steps = true;
	}

	void pause() {
		_process_steps = false;
	}

	bool paused() const {
		return !_process_steps;
	}

	bool step(size_t cpu_steps = 1) {
		bool interrupted = false;
		_breakpoint_manager.clear_reached_state();
		for(size_t i = 0; i < cpu_steps; i++) {
			_cpu.step();
			uint32_t addr = _cpu.regs().get_pc();
			BreakpointManager::MaybeBreakpoint bp = _breakpoint_manager.breakpoint_at(addr);
			bool breakpoint_found = bp.second;
			auto& breakpoint = bp.first->second;
			if(breakpoint_found && breakpoint.enabled()) {
				breakpoint.set_reached(true);
				interrupted = true;
				break;
			}
		}
		update();
		return interrupted;
	}

	bool timer_event() override {
		if(paused()) return false;
		if(step(100)) {
			pause();
		} else {
			resume();
		}
		return Widget::timer_event();
	}

	void update() override  {
		_main_content_layout.update();
		_disasm_view.update();
		_registers_view.update();
		_breakpoint_manager_view.update();
		//_memory_browser.update();
		Widget::update();
	}
};


#endif //MICROMACHINE_EMU_MAINWINDOW_HPP
