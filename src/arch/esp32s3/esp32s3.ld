ENTRY(_start)

MEMORY {
    IRAM (rx) : ORIGIN = 0x40000000, LENGTH = 512K
    DRAM (rw) : ORIGIN = 0x3FC80000, LENGTH = 512K
}

_stack_top = ORIGIN(DRAM) + LENGTH(DRAM);

SECTIONS {
    .text : {
    	*(.literal*)
        *(.text*)
    } > IRAM

    .rodata : {
        *(.rodata*)
    } > IRAM

    __data_load = LOADADDR(.data);

    .data : {
        __data_start = .;
        *(.data*)
        __data_end = .;
    } > DRAM AT > IRAM

    .bss : {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    } > DRAM
}
