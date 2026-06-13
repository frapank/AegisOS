[bits 32]

global _start
extern bootenv_entry

_start:
    cli

    call bootenv_entry

.halt:
    hlt
    jmp .halt
