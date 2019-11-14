

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
#include <cppurses/widget/layouts/horizontal.hpp>
#include <cppurses/widget/widgets/menu_stack.hpp>
#include <cppurses/widget/widgets/titlebar.hpp>
#include <cppurses/widget/widgets/text_display.hpp>

class DisasmView : public cppurses::layout::Vertical {
public:
	cppurses::Text_display& disasm_text{this->make_child<cppurses::Text_display>()};
	DisasmView() {
		disasm_text.set_alignment(cppurses::Alignment::Left);
		disasm_text.set_contents("nothing");
		disasm_text.width_policy.minimum(20);
		disasm_text.width_policy.maximum(40);
		disasm_text.border.enable();
	}
};

class Main_menu : public cppurses::layout::Horizontal {
public:
	DisasmView& disasm_view{this->make_child<DisasmView>()};

	Main_menu() {
		this->focus_policy = cppurses::Focus_policy::Direct;
		this->set_name("Main_menu - head widget");
	}
};

int main(int argc, const char** argv) {

	cppurses::System sys;
	Main_menu demo_menu;
	cppurses::System::set_initial_focus(&demo_menu.disasm_view);
	return sys.run(demo_menu);
}
