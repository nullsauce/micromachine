

#include "lib/cpu.hpp"

#include <fstream>




bool read_file(const std::string& path, std::vector<uint8_t>& buffer) {
	std::ifstream file(path, std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	if(size == -1) return false;
	file.seekg(0, std::ios::beg);
	buffer.clear();
	buffer.resize(size);
	file.read((char *)buffer.data(), size);
	return true;
}


static bool is_32bit(const halfword& instruction) {
	return 0b111 == instruction.uint(13, 3) &&
		   0b00 !=  instruction.uint(11, 2);
}


int main() {

	/*
	std::vector<uint16_t> values;
	for(uint16_t instr = 0; instr < 0xFFFF; instr++) {
		if(is_32bit(instr)) {
			for(uint16_t k = 0; k < 0xFFFF; k+=0xFFF) {
				values.push_back(instr);
				values.push_back(k);
			}
		} else {
			values.push_back(instr);
		}
	}
	std::ofstream file ("/tmp/all_instructions.bin", std::ios::binary);
    file.write ((char *)values.data(), sizeof(uint16_t)*values.size());
    file.close ();
	fprintf(stderr, "size: %d\n", sizeof(uint16_t)*values.size());

	std::vector<uint8_t> buff;
	if(!read_file("/home/fla/projects/micromachine/sdk/build/prog1_bin.bin", buff)) {
		fprintf(stderr, "failed to open file\n");
		return -1;
	}*/

	unsigned char test_bin[] = {
	  0x80, 0xb5, 0x82, 0xb0, 0x00, 0xaf, 0x02, 0x20, 0x04, 0x21, 0x00, 0xf0,
	  0x05, 0xf8, 0x03, 0x1c, 0x7b, 0x60, 0xbd, 0x46, 0x02, 0xb0, 0x80, 0xbd,
	  0x80, 0xb5, 0x82, 0xb0, 0x00, 0xaf, 0x78, 0x60, 0x39, 0x60, 0x7a, 0x68,
	  0x3b, 0x68, 0xd3, 0x18, 0x18, 0x1c, 0xbd, 0x46, 0x02, 0xb0, 0x80, 0xbd
	};




	cpu c;

	c.mem().map(test_bin, 0, sizeof(test_bin));
	//c.mem().map(buff.data(), 0, buff.size());

	c.reset();

	size_t faults = 0;
	for(int i = 0; i < 20000; i++) {
		faults += c.step();
		if(faults >= 2) {
			fprintf(stderr, "Too many faults. Aborting\n");
			break;
		}
	}

	c.regs().print();

	return 0;
}