
#include <cxxopts.hpp>

#include "cpu.hpp"
#include "mcu.hpp"
#include "peripherals/iodev.hpp"
#include "programmer.hpp"
#include "gdb-server.hpp"

#include <chrono>
#include <unistd.h>


int main(int argc, char** argv) {

	bool testing_enabled = false;
	std::string executable_path;
	std::string usart_input_string;
	bool enable_gdb_server = false;
	uint16_t gdb_server_port = 2345;

	cxxopts::Options options("micromachine", "ARMv6-M emulator");
	options.add_options()
		("t,testing", "Enable testing", cxxopts::value<bool>()->default_value("false"))
		("executable", "Executable path", cxxopts::value<std::string>())
		("usart-input-string", "usart input data to be passed to the application", cxxopts::value<std::string>())
		("gdb-server", "Enable gdb server debugging", cxxopts::value<bool>()->default_value("true"))
		("gdb-server-port", "sets the gdb server port (implies gdb-server)", cxxopts::value<uint16_t>()->default_value("2345"))
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

		if(result.count("usart-input-string")) {
			usart_input_string = result["usart-input-string"].as<std::string>();
		}

		testing_enabled = result["testing"].as<bool>();
		executable_path = result["executable"].as<std::string>();
		enable_gdb_server = result["gdb-server"].as<bool>();
		gdb_server_port = result["gdb-server-port"].as<uint16_t>();

	} catch (const cxxopts::OptionParseException& e) {
		fprintf(stderr, "%s\n", e.what());
		std::cout << options.help() << std::endl;
		return EXIT_FAILURE;
	}

	micromachine::system::mcu mcu;
	micromachine::system::programmer::program::ptr program = micromachine::system::programmer::load_elf(executable_path.c_str(), mcu.get_memory(), testing_enabled);

	if(program->is_null()) {
		fprintf(stderr, "Error: Failed to load the given ELF file %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	if(usart_input_string.size()) {
		mcu.get_usart_controller().write_string(usart_input_string);
	}

	iopump iopump(mcu.get_usart_controller(), [](uint8_t byte){
		if(0 == write(STDOUT_FILENO, &byte, 1)) {
			fprintf(stderr, "failed to write to stdout\n");
		}
	});

	mcu.set_io_callback([](uint8_t op, uint8_t data) {
		if(0 == write(STDOUT_FILENO, &data, 1)) {
			fprintf(stderr, "failed to write to stdout\n");
		}
	});

	mcu.reset(program->entry_point());

	if(enable_gdb_server) {
		fprintf(stderr, "Spawning gdb server on port %u\n", gdb_server_port);
		gdb_server(mcu.get_cpu(), program);
	} else {
		auto start = std::chrono::steady_clock::now();
		decltype(start) end;
		for(;;) {
			micromachine::system::cpu::step_result state = mcu.step();
			if(state == micromachine::system::cpu::step_result::BREAK ||
			   state == micromachine::system::cpu::step_result::FAULT) {
				end = std::chrono::steady_clock::now();
				break;
			}
		}
		uint64_t elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
		double elapsed_secs = elapsed_ms / 1000.0;
		fprintf(stderr, "elapsed: %f seconds\n", elapsed_secs);

		uint64_t instructions_executed = mcu.get_cpu().debug_instruction_counter();
		double perf = instructions_executed / elapsed_secs;
		fprintf(stderr, "run %ld instruction(s), %f i/s\n", instructions_executed, perf);
	}

	iopump.shutdown();

	return EXIT_SUCCESS;
}
