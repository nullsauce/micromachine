/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#pragma once


#include <memory>
#include <cppurses/widget/widget.hpp>
#include <cppurses/system/focus.hpp>

namespace {
	template<typename TO, typename FROM>
	std::unique_ptr<TO> static_unique_pointer_cast (std::unique_ptr<FROM>&& old){
		return std::unique_ptr<TO>{static_cast<TO*>(old.release())};
		//conversion: unique_ptr<FROM>->FROM*->TO*->unique_ptr<TO>
	}
}
template<class Widg_t>
class HidableElement {
private:
	std::unique_ptr<Widg_t> _menu_widget;
	cppurses::Widget* _parent;
	size_t _insert_index;
	bool _visible;
public:

	template<typename... Args>
	HidableElement(cppurses::Widget* parent, size_t insert_index, Args&&... args)
		: _menu_widget(std::make_unique<Widg_t>(std::forward<Args>(args)...))
		, _parent(parent)
		, _insert_index(insert_index)
		, _visible(false)
	{}

	void show() {
		if(_menu_widget) {
			_parent->children.insert(std::move(_menu_widget), _insert_index);
			_visible = true;
			cppurses::Focus::set_focus_to(getWidget());
		}
	}

	void hide() {
		if(_parent->children.get().size() > 0) {
			_menu_widget = static_unique_pointer_cast<Widg_t, cppurses::Widget>(_parent->children.remove(&getWidget()));
			_visible = false;
			cppurses::Focus::set_focus_to(getWidget());
		}
	}

	Widg_t& getWidget() {
		if(_visible) {
			return *reinterpret_cast<Widg_t*>(_parent->children.get()[_insert_index].get());
		} else {
			return *_menu_widget.get();
		}
	}

};

