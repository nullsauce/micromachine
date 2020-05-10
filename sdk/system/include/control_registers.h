
#ifndef MICROMACHINE_EMU_CONTROL_REGISTERS_H
#define MICROMACHINE_EMU_CONTROL_REGISTERS_H

#define REG32(address) (*((volatile unsigned long*)address))

#define IO_REG          		REG32(0xE000EF90)

#endif //MICROMACHINE_EMU_CONTROL_REGISTERS_H
