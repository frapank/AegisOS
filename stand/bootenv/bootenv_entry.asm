[bits 32]
jmp start

section .entry
global start
extern bootenv_entry

start:
    cli

    call bootenv_entry

.halt:
    hlt
    jmp .halt


times 8192 - ($-$$) db 0
