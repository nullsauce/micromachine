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


