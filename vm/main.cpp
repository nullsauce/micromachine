
#include <cxxopts.hpp>

#include "mcu.hpp"
#include "peripherals/iodev.hpp"
#include "programmer.hpp"
#include "gdb-server.hpp"
#include "stream_server.hpp"

#include <chrono>
#include <unistd.h>

static const char* OPT_EXECUTABLE = "executable";
static const char* OPT_TESTING = "testing";
static const char* OPT_USART_INPUT_STR = "usart-input-string";
static const char* OPT_USART_INPUT_FILE = "usart-input-file";
static const char* OPT_GDB_SERVER = "gdb-server";
static const char* OPT_GDB_SERVER_PORT = "gdb-server-port";

int main(int argc, char** argv) {

	bool testing_enabled = false;
	std::string executable_path;
	std::string usart_input_string;
	std::string usart_input_file_path;
	bool enable_gdb_server = false;
	uint16_t gdb_server_port = 2345;

	cxxopts::Options options("micromachine", "ARMv6-M emulator");
	options.add_options()
		(OPT_EXECUTABLE, "Executable path", cxxopts::value<std::string>())
		(OPT_TESTING, "Enable testing", cxxopts::value<bool>()->default_value("false"))
		(OPT_USART_INPUT_STR, "usart input data to be passed to the application", cxxopts::value<std::string>()->default_value(""))
		(OPT_USART_INPUT_FILE, "file containing usart input data to be passed to the application", cxxopts::value<std::string>()->default_value(""))
		(OPT_GDB_SERVER, "Enable gdb server debugging", cxxopts::value<bool>()->default_value("false"))
		(OPT_GDB_SERVER_PORT, "sets the gdb server port (implies gdb-server)", cxxopts::value<uint16_t>()->default_value("2345"))
		("h,help", "Print usage")
	;

	try {
		options.parse_positional({OPT_EXECUTABLE});
		auto result = options.parse(argc, argv);

		if (result.count("help")) {
			std::cout << options.help() << std::endl;
			return EXIT_SUCCESS;
		}

		if(result.count(OPT_EXECUTABLE) != 1) {
			std::cout << options.help() << std::endl;
			return EXIT_FAILURE;
		}

		if(result.count(OPT_USART_INPUT_STR) && result.count(OPT_USART_INPUT_FILE)) {
			throw cxxopts::OptionParseException("Can't specify both " +
												std::string(OPT_USART_INPUT_STR) +
												" and " +
												OPT_USART_INPUT_FILE);
		}

		usart_input_string = result[OPT_USART_INPUT_STR].as<std::string>();
		usart_input_file_path = result[OPT_USART_INPUT_FILE].as<std::string>();

		testing_enabled = result[OPT_TESTING].as<bool>();
		executable_path = result[OPT_EXECUTABLE].as<std::string>();
		enable_gdb_server = result[OPT_GDB_SERVER].as<bool>();
		gdb_server_port = result[OPT_GDB_SERVER_PORT].as<uint16_t>();

	} catch (const cxxopts::OptionParseException& e) {
		fprintf(stderr, "Error: %s\n", e.what());
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

	if(usart_input_file_path.size()) {

		std::ifstream usart_input_file(usart_input_file_path, std::ios::binary);
		if(!usart_input_file) {
			fprintf(stderr, "Error: Failed to open usart-input-file=%s\n", usart_input_file_path.c_str());
			return EXIT_FAILURE;
		}

		char byte;
		while(usart_input_file.read(&byte, 1)) {
			mcu.get_usart_controller().send(byte);
		}
	}

	std::unique_ptr<stream_server> usart_streamer =
		create_stream_server(mcu.get_usart_controller(), "usart0", "/tmp/micromachine");

	if(usart_streamer == nullptr) {
		fprintf(stderr, "Error: usart streamer cannot be started\n");
		return EXIT_FAILURE;
	}

	// This client print (stdout) everything is written in USART0->TX register.
	std::unique_ptr<stream_connection> usart_printer = create_stream_connection(
		usart_streamer->pathname(),
		nullptr,
		[](const uint8_t* buffer, size_t size, void*) -> void {
			size_t index = 0;
			while(index < size) {
				if(0 == write(STDOUT_FILENO, &buffer[index], 1)) {
					fprintf(stderr, "failed to write to stdout\n");
				}
				index++;
			}
		},
		nullptr);

	if(usart_printer == nullptr) {
		fprintf(stderr, "Error: usart printer connection failed ('%s')\n", usart_streamer->pathname().c_str());
		return EXIT_FAILURE;
	}

	mcu.set_io_callback([](uint8_t data) {
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
			micromachine::system::mcu::step_result state = mcu.step();
			if(state == micromachine::system::mcu::step_result::HALT) {
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

	if (usart_printer) {
		usart_printer->close();
	}

	if (usart_streamer) {
		usart_streamer->stop();
	}
	return EXIT_SUCCESS;
}
