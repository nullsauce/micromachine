/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EXECUTIONCONTROLLER_HPP
#define MICROMACHINE_EXECUTIONCONTROLLER_HPP

#include <cpu.hpp>
#include "BreakpointManager.hpp"

class ExecutionController {
private:
	cpu& _cpu;
	BreakpointManager& _breakpoint_manager;
	bool _process_steps;
	size_t _step_size;

public:
	sig::Signal<void()> update_required;

	ExecutionController(cpu& cpu, BreakpointManager& breakpoint_manager)
		: _cpu(cpu)
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

			cpu::State state = _cpu.step();
			if(state == cpu::State::BREAK || state == cpu::State::FAULT) {
				break;
			}

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

		return interrupted;
	}


};


#endif //MICROMACHINE_EXECUTIONCONTROLLER_HPP
