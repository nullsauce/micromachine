#pragma once

#include <cppurses/painter/glyph_string.hpp>
#include <cppurses/widget/widgets/menu.hpp>
#include <memory/memory.hpp>

class MemorySegmentList : public cppurses::Menu {
public:
	sig::Signal<void(size_t)> segment_selected;
	sig::Signal<void()> input_cancelled;

	MemorySegmentList(const memory& mem)
		: cppurses::Menu("segments") {

		for(size_t i = 0; i < mem.regions().size(); i++) {
			append_item( mem.regions()[i].name()).connect([this, i](){
				segment_selected(i);
			});
		}

	}

	bool key_press_event(const cppurses::Key::State& keyboard) override {
		if(keyboard.key == cppurses::Key::Escape) {
			input_cancelled();
			return false;
		}
		return cppurses::Menu::key_press_event(keyboard);
	}
};

