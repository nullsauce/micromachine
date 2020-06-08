

#pragma once

#include "widgets/SingleLineTextInput.hpp"

#include <regex>

class CommandInput : public SingleLineTextInput {
public:
	sig::Signal<void(const std::string&, const std::vector<std::string>&)> command_parsed;
	CommandInput(const cppurses::Glyph_string& placeholder)
		: SingleLineTextInput(placeholder){
		text_entered.connect(std::bind(&CommandInput::handle_command, this, std::placeholders::_1));
	}

	void register_command(const std::string& command, const std::regex& pattern, const std::string& description = "") {
		_commands.push_back({command, description, pattern});
	}

private:
	struct command {
		const std::string name;
		const std::string description;
		const std::regex pattern;
	};

	std::vector<command> _commands;

	bool handle_command(const cppurses::Glyph_string& text) {
		for(const command& c : _commands) {
			std::smatch match;
			std::string input = text.str();
			if(std::regex_match(input, match, c.pattern)) {
				if(match.size() < 2) continue;
				std::vector<std::string> groups;
				for(size_t i = 2; i < match.size(); i++) {
					groups.push_back(match[i]);
				}
				command_parsed(match[1], groups);
			}
		}
		return false;
	}
};

