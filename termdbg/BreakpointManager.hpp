

#pragma once

#include <map>
#include <signals/signal.hpp>

class Breakpoint {
private:
	const uint32_t _address;
	bool _enabled;
	bool _reached;

public:
	Breakpoint(uint32_t address)
		: _address(address)
		, _enabled(true)
		, _reached(false) {

	}

	bool enabled() const {
		return _enabled;
	}

	bool reached() const {
		return _reached;
	}

	void toggle() {
		_enabled = !_enabled;
	}

	void set_reached(bool reached) {
		_reached = reached;
	}

	uint32_t address() const {
		return _address;
	}
};

class BreakpointManager {
public:
	using BreakpointMap = std::map<uint32_t, Breakpoint>;
	using MaybeBreakpoint = std::pair<BreakpointMap::iterator, bool>;
	sig::Signal<void()> changed;

private:
	BreakpointMap _breakpoints;
	Breakpoint& create_breakpoint(uint32_t address) {
		auto it = _breakpoints.emplace(address, address);
		return it.first->second;
	}

public:

	MaybeBreakpoint breakpoint_at(uint32_t addr) {
		auto bkp = _breakpoints.find(addr);
		if(bkp != _breakpoints.end()) {
			return {bkp, true};
		} else {
			return {_breakpoints.end(), false};
		}
	}

	Breakpoint& nth_breakpoint(size_t index) {
		auto it = _breakpoints.begin();
		std::advance(it, index);
		return it->second;
	}

	void toggle_breakpoint_at(uint32_t address) {
		auto maybe_breakpoint = breakpoint_at(address);
		if(maybe_breakpoint.second) {
			maybe_breakpoint.first->second.toggle();
			changed();
		}
	}

	void create_breakpoint_at(uint32_t address) {
		auto maybe_breakpoint = breakpoint_at(address);
		if(!maybe_breakpoint.second) {
			create_breakpoint(address);
			changed();
		}
	}

	void destroy_breakpoint_at(uint32_t address) {
		auto maybe_breakpoint = breakpoint_at(address);
		if(maybe_breakpoint.second) {
			_breakpoints.erase(maybe_breakpoint.first);
			changed();
		}
	}

	void clear_reached_state() {
		for(auto& bp : _breakpoints) {
			bp.second.set_reached(false);
		}
	}

	size_t breakpoint_count() const {
		return _breakpoints.size();
	}

	BreakpointMap::const_iterator begin() const {
		return _breakpoints.begin();
	}

	BreakpointMap::const_iterator end() const {
		return _breakpoints.end();
	}
};


