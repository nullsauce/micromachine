arm-none-eabi-as -march=armv6-m -mthumb test.s -o test.o && \
arm-none-eabi-objcopy -O binary test.o test.bin && \
arm-none-eabi-ld test.o -o test.elf && \
xxd -i test.bin && \
arm-none-eabi-objdump -b binary -D -marm -Mforce-thumb test.bin
