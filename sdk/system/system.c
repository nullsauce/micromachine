/* Copyright (C) Flavio Roth - All Rights Reserved

* Unauthorized copying of this file, via any medium is strictly prohibited.

* This project is proprietary and confidential. It can not be copied
and/or distributed without the express permission of Flavio Roth.

*/

#include "include/system.h"

extern char __heap_start;
extern char __heap_end;
extern char __heap_size;

extern char __bss_start__;
extern char __bss_end__;
extern char __bss_size__;

extern char __data_start__;
extern char __data_end__;
extern char __data_size__;

void* _system_heap_start = &__heap_start;
void* _system_heap_end = &__heap_end;
uint32_t _system_heap_size = (uint32_t)&__heap_size;

void* _system_bss_start = &__bss_start__;
void* _system_bss_end = &__bss_end__;
uint32_t _system_bss_size = (uint32_t)&__bss_size__;

void* _system_data_start = &__data_start__;
void* _system_data_end = &__data_end__;
uint32_t _system_data_size = (uint32_t)&__data_size__;
