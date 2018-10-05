

#ifndef MICROMACHINE_SDK_SYSTICK_H
#define MICROMACHINE_SDK_SYSTICK_H

#include "control_registers.h"

#define NVIC_ST_CTRL_COUNT      0x00010000  // Count flag
#define NVIC_ST_CTRL_CLK_SRC    0x00000004  // Clock Source
#define NVIC_ST_CTRL_INTEN      0x00000002  // Interrupt enable
#define NVIC_ST_CTRL_ENABLE     0x00000001  // Counter mode
#define NVIC_ST_RELOAD_M        0x00FFFFFF  // Counter load value

#endif //MICROMACHINE_SDK_SYSTICK_H
