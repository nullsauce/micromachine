

#include "lib/cpu.hpp"

int main() {

	unsigned char test_bin[] = {
			0x40, 0x40, 0x14, 0x28, 0x03, 0xd0, 0x02, 0x23, 0x98, 0x46, 0x01, 0x30,
			0x40, 0x47
	};


	cpu c;

	c.mem().map(test_bin, 0, sizeof(test_bin));

	for(int i = 0; i < 20000; i++) {
		c.step();
	}

	c.regs().print();

	return 0;
}