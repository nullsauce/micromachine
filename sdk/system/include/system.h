/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_SYSTEM_H
#define MICROMACHINE_SYSTEM_H

#include <stdint.h>

extern void* _system_heap_start;
extern void* _system_heap_end;
extern uint32_t _system_heap_size;

extern void* _system_bss_start;
extern void* _system_bss_end;
extern uint32_t _system_bss_size;

#endif //MICROMACHINE_SYSTEM_H
