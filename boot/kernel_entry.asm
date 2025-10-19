;----------------
; Start the kernel
global _start
extern kernel_main

section .text
    bits 32

_start:
    call kernel_main

    cli
.halt_loop:
    hlt
    jmp .halt_loop

