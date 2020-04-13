
/* Specify the memory areas */
MEMORY
{
    ROM (rx)        : ORIGIN = 0x00000000, LENGTH = 0x10000 /*64K*/
    RAM (rwx)       : ORIGIN = 0x20000000, LENGTH = 0x04000 /*16K*/
}

/* Specify the segments */


/* Define stack size and heap size here */
__heap_size__ = 0x1000;
__stack_size = 0x2000;

/* Specify the entry point */
ENTRY(_startup)

/* Define output sections */
SECTIONS
{
    /* The startup code goes first into ROM */
    .isr_vector :
    {
        . = ALIGN(4);
        __isr_vector_start = .;
        KEEP(*(.isr_vector)) /* without 'KEEP' the garbage collector discards this section */
        ASSERT(. != __isr_vector_start, "The .isr_vector section is empty");
        __isr_vector_end = .;
        . = ALIGN(4);
    } > ROM

    /* The program code and other data goes into ROM */
    .text :
    {
        . = ALIGN(4);
        _stext = .;
        *(.text)           /* .text sections (code) */
        *(.text.startup)   /* .text.startup (is used for static constructors and main()) */
        *(.text*)          /* .text* sections (code) */
        *(.rodata)         /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
        *(.glue_7)         /* glue arm to thumb code */
        *(.glue_7t)        /* glue thumb to arm code */
        . = ALIGN(4);
        _etext = .;        /* define a global symbols at end of code */
    } > ROM

    /* used by the startup to initialize data */
    __init_data_start__ = LOADADDR(.data);


    /* Initialized data sections goes into RAM, load LMA copy after code */
    /* Aligned to 32 bit so that copy can be done with 32 bit words */
    .data :
    {
        . = ALIGN(4);
        __data_start__ = .; /* create a global symbol at data start */
        *(.data)            /* .data sections */
        *(.data*)           /* .data* sections */
        . = ALIGN(4);
        __data_end__ = .;   /* define a global symbol at data end */

    } > RAM AT >ROM

    /* Compute size and end of section */
    __data_size__ = __data_end__ - __data_start__;
    __init_data_size__ = __data_size__;
    __init_data_end__ = __init_data_start__ + __init_data_size__;

    /* Uninitialized data section */
    /* Aligned to 32 bit so that copy can be done with 32 bit words */
    . = ALIGN(4);
    .bss (NOLOAD) :
    {
        /*  Used by the startup in order to initialize the .bss secion */
        __bss_start__ = .;  /* define a global symbol at bss start */
        *(.bss)
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end__ = .;    /* define a global symbol at bss end */
    } > RAM

    __bss_size__ = __bss_end__ - __bss_start__;

    . = ALIGN(4);
    .heap (NOLOAD) :
    {
       __heap_start__ = .;
       . = . + __heap_size__;
       __heap_end__ = .;
    } > RAM

    . = ALIGN(4);

    _ram_end_ = ORIGIN(RAM) + LENGTH(RAM);

    /* stack with a fixed size */
    .stack (NOLOAD) :
    {
        . = ALIGN(4);
        _stack_start = .;
        . = . + __stack_size;
        . = ALIGN(4);
        _stack_end = .;
    } > RAM

    _stack_bottom = _stack_end - 1;
}
