/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EXECUTIONCONTROLLER_HPP
#define MICROMACHINE_EXECUTIONCONTROLLER_HPP

#include "BreakpointManager.hpp"
#include "cpu.hpp"
#include <mcu.hpp>

class ExecutionController {
private:
	mcu& _mcu;
	uint32_t _entry_point;
	BreakpointManager& _breakpoint_manager;
	bool _process_steps;
	size_t _step_size;

public:
	sig::Signal<void()> update_required;

	ExecutionController(mcu& system, uint32_t entry_point, BreakpointManager& breakpoint_manager)
		: _mcu(system)
		, _entry_point(entry_point)
		, _breakpoint_manager(breakpoint_manager)
		, _process_steps(false)
		, _step_size(1000)
	{}

	void initialize() {
		update_required();
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

	void reset() {
		_mcu.reset(_entry_point);
	}

	bool key_press_event(const cppurses::Key::State& keyboard) {
		bool consumed = false;
		if(paused() && keyboard.key == cppurses::Key::s) {
			step();
			consumed = true;
		} else if(paused() && keyboard.key == cppurses::Key::r) {
			resume();
			consumed = true;
		} else if(!paused() && keyboard.key == cppurses::Key::p) {
			pause();
			consumed = true;
		} else if(paused() && keyboard.key == cppurses::Key::Ctrl_r) {
			reset();
			consumed = true;
		}

		if(consumed) {
			update_required();
		}
		return consumed;
	}


	void run_multi_steps() {
		// do not do anything if paused
		if(!paused()) {
			if(step(_step_size)) {
				// step was interrupted.
				// for ex. by hitting a breakpoint
				pause();
			} else {
				// all the steps executed without interruption
				resume();
			}
		}
		update_required();
	}

	bool step(size_t cpu_steps = 1) {

		bool interrupted = false;
		_breakpoint_manager.clear_reached_state();

		for(size_t i = 0; i < cpu_steps; i++) {

			cpu::step_result state = _mcu.step();
			if(state == cpu::step_result::BREAK || state == cpu::step_result::FAULT) {
				interrupted = true;
				break;
			}

			uint32_t addr = _mcu.get_cpu().regs().get_pc();
			BreakpointManager::MaybeBreakpoint bp = _breakpoint_manager.breakpoint_at(addr);
			bool breakpoint_found = bp.second;
			auto& breakpoint = bp.first->second;
			if(breakpoint_found && breakpoint.enabled()) {
				breakpoint.set_reached(true);
				interrupted = true;
				break;
			}
		}

		return interrupted;
	}


};


#endif //MICROMACHINE_EXECUTIONCONTROLLER_HPP
