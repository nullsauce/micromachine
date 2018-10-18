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
	Singleton() {}
private:
	int a;
};

void main()  {
	Singleton a;
	Singleton* b = &a;
	Singleton* c = new Singleton();

}


