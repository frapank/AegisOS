[bits 32]

global _start
extern kernel_main

_start:
    jmp $
    cli ; Will enable the interrupts after the pid_init()
    call kernel_main
    cli
.halt_loop:
    hlt
    jmp .halt_loop

