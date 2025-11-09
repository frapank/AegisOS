[bits 32]
global _start
extern kernel_main

_start:
    ; Cose da fare prima
    call start_kernel

start_kernel:
    cli ; Will enable the interrupts after the pid_init()
    call kernel_main
    cli
.halt_loop:
    hlt
    jmp .halt_loop

