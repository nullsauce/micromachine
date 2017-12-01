arm-none-eabi-gcc -ffreestanding -nodefaultlibs -nostdlib -nostartfiles -mthumb -march=armv6s-m testc.c -o testc.o && \
arm-none-eabi-objcopy -O binary testc.o testc.bin && \
arm-none-eabi-ld testc.o -o testc.elf && \
xxd -i testc.bin && \
arm-none-eabi-objdump -b binary -D -marm -Mforce-thumb testc.bin
