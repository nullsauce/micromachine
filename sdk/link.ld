
/* Specify the memory areas */
MEMORY
{
    FLASH (rx)      : ORIGIN = 0x00000000, LENGTH = 0x10000 /*64K*/
    RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 0x04000 /*16K*/
}

/* define stack size and heap size here */

heap_size = 8*1024;

ENTRY(_isr_reset)

/* Define output sections */
SECTIONS
{
    /* The startup code goes first into FLASH */
    .isr_vector :
    {
        . = ALIGN(4);
        _isr_vector_start = .;
        KEEP(*(.isr_vector)) /* without 'KEEP' the garbage collector discards this section */
        ASSERT(. != _isr_vector_start, "The .isr_vector section is empty");
        _isr_vector_end = .;
        . = ALIGN(4);
    } > FLASH

    /* The program code and other data goes into FLASH */
    .text :
    {
        . = ALIGN(4);
        _stext = .;
        *(.text)           /* .text sections (code) */
        *(.text*)          /* .text* sections (code) */
        *(.rodata)         /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
        *(.glue_7)         /* glue arm to thumb code */
        *(.glue_7t)        /* glue thumb to arm code */
        . = ALIGN(4);
        _etext = .;        /* define a global symbols at end of code */
    } > FLASH

    .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >FLASH
    .ARM : {
        __exidx_start = .;
         *(.ARM.exidx*)
         __exidx_end = .;
    } > FLASH

    /* used by the startup to initialize data */
    _sidata = .;

    /* Initialized data sections goes into RAM, load LMA copy after code */
    .data : AT ( _sidata )
    {
        . = ALIGN(4);
        _sdata = .;        /* create a global symbol at data start */
        *(.data)           /* .data sections */
        *(.data*)          /* .data* sections */

        . = ALIGN(4);
        _edata = .;        /* define a global symbol at data end */
    } >RAM

    /* Uninitialized data section */
    . = ALIGN(4);
    .bss :
    {
        /*  Used by the startup in order to initialize the .bss secion */
        _sbss = .;         /* define a global symbol at bss start */
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);
        _ebss = .;         /* define a global symbol at bss end */
        __bss_end__ = _ebss;
    } > RAM

    . = ALIGN(4);
    .heap (NOLOAD) :
    {
       _heap_start = .;
       . = . + heap_size;
       _heap_end = .;
    } > RAM

    . = ALIGN(4);

    _ram_end_ = ORIGIN(RAM) + LENGTH(RAM);
    _stack_size = _ram_end_ - _heap_end;

    .stack (NOLOAD) :
    {
        . = ALIGN(4);
        _stack_start = .;
        . = . + _stack_size;
        . = ALIGN(4);
        _stack_end = .;
    } > RAM

    _stack_bottom = _stack_end - 1;
    .ARM.attributes 0 : { *(.ARM.attributes) }

    }