

#include <chrono>
#include "cpu.hpp"

int main(int argc, const char** argv) {

	if(argc < 2) {
		fprintf(stderr, "usage: %s <elf-executable>\n", argv[0]);
		return EXIT_FAILURE;
	}

	cpu c;
	c.load_elf(argv[1]);

	for(int i = 0; i < 40; i++) {
	c.reset();

	auto start = std::chrono::steady_clock::now();
	decltype(start) end;
	for(;;) {
		bool fault = c.step();
		if(fault) {
			end = std::chrono::steady_clock::now();
			fprintf(stderr, "Too many faults. Aborting\n");
			break;
		}
	}
	uint64_t elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	fprintf(stderr, "elapsed: %f seconds\n", elapsed_ms/1000.0);

	}
	//c.regs().print();
	return EXIT_SUCCESS;
}