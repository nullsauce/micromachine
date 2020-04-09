
#ifndef MICROMACHINE_EMU_MEM_H
#define MICROMACHINE_EMU_MEM_H

#include <stddef.h>

void* memset(void* m, int c, size_t n);
void* memcpy(void* __restrict__ out, const void* __restrict__ in, size_t n);

#endif //MICROMACHINE_EMU_MEM_H
