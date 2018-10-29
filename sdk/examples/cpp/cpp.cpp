/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include <interrupt_handlers.h>
#include <instructions.h>
#include <systick.h>
#include <stdint.h>
#include <stddef.h>
#include <tinyprintf.h>
#include <entrypoint.h>
#include <random.h>

extern char _heap_start;

void io_call(uint8_t op, uint8_t d0, uint8_t d1, uint8_t d2) {
	IO_REG = ((op << 24) | (d2 << 16) | (d2 << 8) | (d0 << 0));
}

void printf_putc(void* ptr, char c) {
	io_call(0, c, 0, 0);
}



class Singleton {
public:
	static Singleton& get() {
		static Singleton instance;
		return instance;
	}
	int val() const {
		return _val;
	}
private:
	Singleton()
		: _val(42) {
		printf("Singleton::Singleton()\n");
	}
	~Singleton() {
		printf("Singleton::Singleton()\n");
	}
	int _val;
};

void main()  {
	init_printf(NULL, printf_putc);
	printf("Singleton::get() = %i\n", Singleton::get().val());
}


