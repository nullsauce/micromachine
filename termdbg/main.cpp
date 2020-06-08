

#include <cppurses/cppurses_system.hpp>

#include "loader.hpp"
#include "mcu.hpp"

using namespace micromachine::system;

#include "widgets/MainWindow.hpp"

#include <cstdlib>

int main(int argc, const char** argv) {

	if(argc < 2) {
		fprintf(stderr, "usage: %s <elf-executable>\n", argv[0]);
		return EXIT_FAILURE;
	}

	micromachine::system::mcu mcu;

	loader::program::ptr program = loader::load_elf(argv[1], mcu.get_memory(), false);

	if(program->is_null()) {
		fprintf(stderr, "Error: Failed to load the given ELF file %s\n", argv[0]);
		return EXIT_FAILURE;
	}

	mcu.reset(program->entry_point());

	cppurses::System sys;

	MainWindow main_window(mcu, program->entry_point());
	main_window.width_policy.min_size(100);
	cppurses::System::set_initial_focus(&main_window.disasm_view());

	return sys.run(main_window);
}
