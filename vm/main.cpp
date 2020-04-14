
#include <cxxopts.hpp>

#include "cpu.hpp"
#include "programmer.hpp"

#include <chrono>
#include <unistd.h>


int main(int argc, char** argv) {

	bool testing_enabled = false;
	std::string executable_path;

	cxxopts::Options options("micromachine", "ARMv6-M emulator");
	options.add_options()
		("t,testing", "Enable testing", cxxopts::value<bool>()->default_value("false"))
		("executable", "Executable path", cxxopts::value<std::string>())
		("h,help", "Print usage")
	;

	try {
		options.parse_positional({"executable"});
		auto result = options.parse(argc, argv);

		if (result.count("help")) {
			std::cout << options.help() << std::endl;
			return EXIT_SUCCESS;
		}

		if(result.count("executable") != 1) {
			std::cout << options.help() << std::endl;
			return EXIT_FAILURE;
		}

		testing_enabled = result["testing"].as<bool>();
		executable_path = result["executable"].as<std::string>();

	} catch (const cxxopts::OptionParseException& e) {
		fprintf(stderr, "%s\n", e.what());
		std::cout << options.help() << std::endl;
		return EXIT_FAILURE;
	}

	cpu cpu;
	programmer::program::ptr program = programmer::load_elf(executable_path.c_str(), cpu.mem(), testing_enabled);

	if(program->is_null()) {
		fprintf(stderr, "Error: Failed to load the given ELF file %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	cpu.set_io_callback([](uint8_t op, uint8_t data){
		if(0 == write(STDOUT_FILENO, &data, 1)) {
			fprintf(stderr, "failed to write to stdout\n");
		}
	});

	cpu.reset(program->entry_point());

	auto start = std::chrono::steady_clock::now();
	decltype(start) end;
	for(;;) {
		cpu::State state = cpu.step();
		if(state == cpu::State::BREAK || state == cpu::State::FAULT) {
			end = std::chrono::steady_clock::now();
			break;
		}
	}
	uint64_t elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	double elapsed_secs = elapsed_ms/1000.0;
	fprintf(stderr, "elapsed: %f seconds\n", elapsed_secs);

	double perf = cpu.debug_instruction_counter() / elapsed_secs;
	fprintf(stderr, "run %ld instruction(s), %f i/s\n", cpu.debug_instruction_counter(), perf);
	//c.regs().print();
	return EXIT_SUCCESS;
}
