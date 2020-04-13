/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_STDIO_H
#define MICROMACHINE_STDIO_H

#include <tinyprintf.h>

void _putc();
void _printf(char *fmt, ...);
void _init_io();

#endif //MICROMACHINE_STDIO_H
