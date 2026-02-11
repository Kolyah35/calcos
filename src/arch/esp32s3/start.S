    .section .text
    .global _start
    .type _start, @function

_start:
    movi    a1, _stack_top
    movi    a2, __bss_start
    movi    a3, __bss_end

bss_loop:
    beq     a2, a3, bss_done
    s32i    a0, a2, 0
    addi    a2, a2, 4
    j       bss_loop

bss_done:
    movi    a2, __data_load
    movi    a3, __data_start
    movi    a4, __data_end

data_loop:
    beq     a3, a4, data_done
    l32i    a5, a2, 0
    s32i    a5, a3, 0
    addi    a2, a2, 4
    addi    a3, a3, 4
    j       data_loop

data_done:
    call0   kernel_main

halt:
    j halt
