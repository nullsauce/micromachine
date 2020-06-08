

#pragma once

#include <cppurses/painter/brush.hpp>
#include <cppurses/cppurses_painter.hpp>
#include <cppurses/widget/widgets/text_display.hpp>

#include <vector>

class FormatedText {
private:
	cppurses::Glyph_string _glyph_str;
	cppurses::Brush _current_brush;
public:

	FormatedText() {
		reset();
	}
	template<typename... Attributes>
	FormatedText& write(cppurses::Glyph_string str, Attributes&&... args) {
		_current_brush.add_attributes(std::forward<Attributes>(args)...);
		apply_brush(str);
		_glyph_str.append(std::move(str));
		_current_brush.remove_attributes(std::forward<Attributes>(args)...);
		return *this;
	}

	template<typename... Attributes>
	FormatedText& write_color_f(cppurses::Glyph_string str, cppurses::Color foreground, Attributes&&... args) {
		auto previous_foreground = _current_brush.foreground_color();
		set_foreground(foreground);
		write(std::move(str), std::forward<Attributes>(args)...);
		if(previous_foreground) {
			set_foreground(previous_foreground.get());
		} else {
			_current_brush.remove_foreground();
		}
		return *this;
	}

	template<typename... Attributes>
	FormatedText& write_color_fb(cppurses::Glyph_string str, cppurses::Color foreground, cppurses::Color
	background, Attributes&&... args) {
		auto previous_background = _current_brush.background_color();
		set_background(background);
		write_color_f(std::move(str), foreground, std::forward<Attributes>(args)...);
		if(previous_background) {
			set_background(previous_background.get());
		} else {
			_current_brush.remove_background();
		}
		return *this;
	}

	template<typename... Attributes>
	FormatedText& set_attribute(Attributes&&... args) {
		_current_brush.add_attributes(std::forward<Attributes>(args)...);
		return *this;
	}

	template<typename... Attributes>
	FormatedText& remove_attribute(Attributes&&... args) {
		_current_brush.remove_attributes(std::forward<Attributes>(args)...);
		return *this;
	}

	FormatedText& clear_attributes() {
		_current_brush.clear_attributes();
		return *this;
	}

	FormatedText& set_foreground(cppurses::Color& color) {
		_current_brush.set_foreground(color);
		return *this;
	}

	FormatedText& set_background(cppurses::Color& color) {
		_current_brush.set_background(color);
		return *this;
	}

	FormatedText& reset_colors() {
		_current_brush.remove_background();
		_current_brush.remove_foreground();
		return *this;
	}

	cppurses::Glyph_string str() const {
		return _glyph_str;
	}


private:
	void apply_brush(cppurses::Glyph_string& str) {
		for(auto& glyph : str) {
			glyph.brush = _current_brush;
		}
	}

	void reset() {
		clear_attributes();
		reset_colors();
	}
};

