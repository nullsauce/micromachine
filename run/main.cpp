

#include "cpu.hpp"

int main(int argc, const char** argv) {

	if(argc < 2) {
		fprintf(stderr, "usage: %s <elf-executable>\n", argv[0]);
		return EXIT_FAILURE;
	}

	cpu c;
	c.load_elf(argv[1]);
	c.reset();

	for(;;) {
		bool fault = c.step();
		if(fault) {
			fprintf(stderr, "Too many faults. Aborting\n");
			break;
		}
	}
	c.regs().print();
	return EXIT_SUCCESS;
}