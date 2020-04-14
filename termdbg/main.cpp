/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include <cppurses/cppurses_system.hpp>

#include "widgets/MainWindow.hpp"
#include "cpu.hpp"
#include "programmer.hpp"

#include <cstdlib>

int main(int argc, const char** argv) {

	if(argc < 2) {
		fprintf(stderr, "usage: %s <elf-executable>\n", argv[0]);
		return EXIT_FAILURE;
	}

	cpu cpu_instance;

	programmer::program::ptr program = programmer::load_elf(argv[1], cpu_instance.mem());

	if(program->is_null()) {
		fprintf(stderr, "Error: Failed to load the given ELF file %s\n", argv[0]);
		return EXIT_FAILURE;
	}

	cpu_instance.reset(program->entry_point());

	cppurses::System sys;

	MainWindow main_window(cpu_instance, program->entry_point());
	main_window.width_policy.min_size(100);
	cppurses::System::set_initial_focus(&main_window.disasm_view());

	return sys.run(main_window);
}
