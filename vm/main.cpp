
#include <cxxopts.hpp>

#include "cpu.hpp"
#include "mcu.hpp"
#include "programmer.hpp"

#include <chrono>
#include <unistd.h>

/**
 * A dummy feeder for usart rx register
 * @return the next dummy data
 */
static uint8_t usart_rx_feeder_next_data() {
	static const std::string data = "I'm your feeder! ";
	static uint8_t index = 0;
	uint8_t d = data[index];
	index = (index + 1) % data.size();
	return d;
}

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

	micromachine::system::mcu mcu;
	micromachine::system::programmer::program::ptr program = micromachine::system::programmer::load_elf(executable_path.c_str(), mcu.get_memory(), testing_enabled);

	if(program->is_null()) {
		fprintf(stderr, "Error: Failed to load the given ELF file %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	mcu.set_io_callback([](uint8_t op, uint8_t data){
		if(0 == write(STDOUT_FILENO, &data, 1)) {
			fprintf(stderr, "failed to write to stdout\n");
		}
	});

	mcu.set_usart_tx_callback([](uint8_t op, uint8_t data) {
		if(0 == write(STDOUT_FILENO, &data, 1)) {
			fprintf(stderr, "failed to write to stdout\n");
		}
	});

	mcu.set_usart_rx_callback([&mcu](uint8_t& data) {
		data = usart_rx_feeder_next_data();
		mcu.set_usart_rx_data(data);
	});

	mcu.reset(program->entry_point());

	// Setup the first byte in usart RX register
	mcu.set_usart_rx_data(usart_rx_feeder_next_data());

	auto start = std::chrono::steady_clock::now();
	decltype(start) end;
	for(;;) {
		micromachine::system::cpu::step_result state = mcu.step();
		if(state == micromachine::system::cpu::step_result::BREAK || state == micromachine::system::cpu::step_result::FAULT) {
			end = std::chrono::steady_clock::now();
			break;
		}
	}
	uint64_t elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	double elapsed_secs = elapsed_ms/1000.0;
	fprintf(stderr, "elapsed: %f seconds\n", elapsed_secs);

	uint64_t instructions_executed = mcu.get_cpu().debug_instruction_counter();
	double perf = instructions_executed / elapsed_secs;
	fprintf(stderr, "run %ld instruction(s), %f i/s\n", instructions_executed, perf);
	//c.regs().print();
	return EXIT_SUCCESS;
}
