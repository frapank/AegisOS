;---------------------------------------------------------------
; This is a simple boot loader for my operating sysetm.
; I hope this project could get bigger :D
;---------------------------------------------------------------

bits 16
org 0x7c00

start:
    ; Set boot disk
    mov [boot_disk], dl

    ; Set Stack
    mov ax, 0x9000
    mov ss, ax
    mov sp, 0xFFFF
    mov bp, sp

    ; Set Data Segment
    mov ax, 0x0000
    mov ds, ax
    mov es, ax

    ; Render
    call clear_screen
    call print_welcome
    call wait_key
    call clear_screen
    call read_disk

    call start_protected

%include "boot/data.asm"
%include "boot/graphics.asm"
%include "boot/gdt.asm"


times 510-($-$$) db 0
dw 0xaa55
