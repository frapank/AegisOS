[bits 32]

extern bootenv_entry

section .text
_start:
    call bootenv_entry

