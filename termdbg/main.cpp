

#include <chrono>
#include <unistd.h>
#include "cpu.hpp"

#include <cppurses/system/events/key.hpp>
#include <cppurses/system/shortcuts.hpp>
#include <cppurses/system/system.hpp>
#include <cppurses/widget/focus_policy.hpp>
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/menu.hpp>
#include <cppurses/system/focus.hpp>
#include <cppurses/widget/layouts/vertical.hpp>
#include <cppurses/widget/widgets/menu_stack.hpp>
#include <cppurses/widget/widgets/titlebar.hpp>

class Main_menu : public cppurses::layout::Vertical {
public:
	cppurses::Titlebar& titlebar{this->make_child<cppurses::Titlebar>("  C  P  P  U  R  S  E  S")};
	cppurses::Menu_stack& main_menu{this->make_child<cppurses::Menu_stack>("Demos")};

	Main_menu() {
		this->focus_policy = cppurses::Focus_policy::Direct;
		this->set_name("Main_menu - head widget");
		titlebar.set_name("Titlebar in Main_menu");
		main_menu.set_name("Menu_stack in Main_menu");
		main_menu.menu().set_name("Menu in Main Widget Stack Menu");

		auto& esc_short = cppurses::Shortcuts::add_shortcut(cppurses::Key::Escape);
		esc_short.connect([this] { main_menu.goto_menu(); });
	}
};

int main(int argc, const char** argv) {
	setlocale(LC_ALL, "");
	cppurses::System sys;
	Main_menu demo_menu;
	cppurses::System::set_initial_focus(&demo_menu.main_menu);
	return sys.run(demo_menu);

}
