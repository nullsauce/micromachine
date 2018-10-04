

#include <chrono>
#include <unistd.h>
#include "cpu.hpp"

int main(int argc, const char** argv) {

	if(argc < 2) {
		fprintf(stderr, "usage: %s <elf-executable>\n", argv[0]);
		return EXIT_FAILURE;
	}

	cpu c;
	c.load_elf(argv[1]);
	c.set_io_callback([](uint8_t op, uint8_t data){
		write(STDOUT_FILENO, &data, 1);
	});

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
	double elapsed_secs = elapsed_ms/1000.0;
	fprintf(stderr, "elapsed: %f seconds\n", elapsed_secs);

	double perf = c.debug_instruction_counter() / elapsed_secs;
	fprintf(stderr, "run %ld instruction(s), %f i/s\n", c.debug_instruction_counter(), perf);
	//c.regs().print();
	return EXIT_SUCCESS;
}
