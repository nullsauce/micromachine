/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_EMU_MEMORYSEGMENTINFO_HPP
#define MICROMACHINE_EMU_MEMORYSEGMENTINFO_HPP

#include <cppurses/widget/widgets/text_display.hpp>
#include <cppurses/widget/layouts/horizontal.hpp>
#include <memory/mapping.hpp>

class MemorySegmentInfo : public cppurses::layout::Horizontal {
private:
	const memory_mapping* _segment {nullptr};
	cppurses::Text_display& _segment_name {make_child<cppurses::Text_display>("")};
	cppurses::Text_display& _segment_size {make_child<cppurses::Text_display>("")};
	cppurses::Text_display& _segment_start {make_child<cppurses::Text_display>("")};
	cppurses::Text_display& _segment_end {make_child<cppurses::Text_display>("")};

public:

	MemorySegmentInfo() {
		border.enable();
		border.segments.disable_all();
		border.segments.south.enable();
		height_policy.fixed(3);

		_segment_name.set_alignment(cppurses::Alignment::Left);
		_segment_size.set_alignment(cppurses::Alignment::Right);
		_segment_start.set_alignment(cppurses::Alignment::Right);
		_segment_end.set_alignment(cppurses::Alignment::Right);

		_segment_size.border.enable();
		_segment_size.border.segments.disable_all();
		_segment_size.border.segments.west.enable();

		_segment_start.border.enable();
		_segment_start.border.segments.disable_all();
		_segment_start.border.segments.west.enable();

		_segment_end.border.enable();
		_segment_end.border.segments.disable_all();
		_segment_end.border.segments.west.enable();
	}

	void set_segment(const memory_mapping* segment) {
		if(_segment != segment) {
			_segment = segment;
			update();
		}
	}

	bool paint_event() override {
		render();
		return Widget::paint_event();
	}

	void render() {
		if(nullptr == _segment) return;
		char buf[64];
		snprintf(buf, sizeof(buf), "segment\n%s", _segment->name().c_str());
		_segment_name.set_contents(buf);

		snprintf(buf, sizeof(buf), "size\n%08x", _segment->size());
		_segment_size.set_contents(buf);

		snprintf(buf, sizeof(buf), "start\n%08x", _segment->start());
		_segment_start.set_contents(buf);

		snprintf(buf, sizeof(buf), "end\n%08x", _segment->end());
		_segment_end.set_contents(buf);
	}

};

#endif //MICROMACHINE_EMU_MEMORYSEGMENTINFO_HPP
