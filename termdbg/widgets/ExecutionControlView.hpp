

#pragma once

#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/push_button.hpp>

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

