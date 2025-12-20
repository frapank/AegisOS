[bits 32]

global _start
extern kernel_main

_start:
    cli                 ; Disable interrupts, will be reenabled after pid_init() in the kernel_main
    push ebx            ; Pass boot_info
    push eax            ; Pass magic_numer
    call kernel_main    ; Call the main kernel function
    cli                 ; Disable interupts again after returning (safety measure)
.halt_loop:
    hlt                 ; Hlt CPU until next interrupt
    jmp .halt_loop      ; Infinite loop to prevent execution past this point
