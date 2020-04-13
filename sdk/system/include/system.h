/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#ifndef MICROMACHINE_SYSTEM_H
#define MICROMACHINE_SYSTEM_H

#include <stdint.h>


// .data section properties
extern char __data_start__;
extern char __data_end__;
extern char __data_size__;

#define _section_data_start ((void*)&__data_start__)
#define _section_data_end ((void*)&__data_end__)
#define _section_data_size ((uint32_t)&__data_size__)
#define _section_data_contains(address) (((uint8_t*)address >= (uint8_t*)_section_data_start) && ((uint8_t*)address < (uint8_t*)_section_data_end))


// initialization for .data section properties
extern char __init_data_start__;
extern char __init_data_end__;
extern char __init_data_size__;

#define _section_init_data_start ((void*)&__init_data_start__)
#define _section_init_data_end ((void*)&__init_data_end__)
#define _section_init_data_size ((uint32_t)&__init_data_size__)
#define _section_init_data_contains(address) (((uint8_t*)address >= (uint8_t*)_section_init_data_start) && ((uint8_t*)address < (uint8_t*)_section_init_data_end))


// .bss section properties
extern char __bss_start__;
extern char __bss_end__;
extern char __bss_size__;

#define _section_bss_start ((void*)&__bss_start__)
#define _section_bss_end ((void*)&__bss_end__)
#define _section_bss_size ((uint32_t)&__bss_size__)
#define _section_bss_contains(address) (((uint8_t*)address >= (uint8_t*)_section_bss_start) && ((uint8_t*)address < (uint8_t*)_section_bss_end))


// .data section properties
extern char __heap_start__;
extern char __heap_end__;
extern char __heap_size__;

#define _section_heap_start ((void*)&__heap_start__)
#define _section_heap_end ((void*)&__heap_end__)
#define _section_heap_size ((uint32_t)&__heap_size__)
#define _section_heap_contains(address) (((uint8_t*)address >= (uint8_t*)_section_heap_start) && ((uint8_t*)address < (uint8_t*)_section_heap_end))


#endif //MICROMACHINE_SYSTEM_H
