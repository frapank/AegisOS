[bits 16]
org 0x1000
jmp entry


; Data
msg_disk_log            db "[*] Searching second bootenv...",0Dh,0Ah,0
msg_disk_error          db "[-] Can't find second bootenv, stopped" ,0Dh,0Ah,0

boot_disk               db 1

CODE_SEG                equ gdt_code - gdt_start
DATA_SEG                equ gdt_data - gdt_start

BOOTENV_LOCATION   equ 0x2000
BOOTENV_SECTORS    equ 16
BOOTENV_START_LBA  equ 8192

CR0_PE                  equ 1

dap:
    db 0x10                             ; packet size
    db 0                                ; reserved
    dw BOOTENV_SECTORS                  ; number of sectors
    dw BOOTENV_LOCATION                 ; offset
    dw 0x0000                           ; segment
    dq BOOTENV_START_LBA                ; LBA start


; Includes
%include "bint/graphics.asm"
%include "bint/read_disk.asm"


; Entry
entry:
    ; boot disk passed by stage1
    mov dl, [0x7E00]
    mov byte [boot_disk], dl

    ; print UI
    mov si, msg_disk_log
    call print_string

    push word [boot_disk]
    push dap

    ; read disk
    call read_disk
    jc disk_error 

    ; start protected mode
    mov ax, 0x2401
    int 0x15

    call start_protected

disk_error:
    mov si, msg_disk_error
    call print_string
    hlt
    jmp $


; Protected mode setup
start_protected:
    cli

    ; clear segments before mode switch
    xor ax, ax
    mov ds, ax
    mov es, ax

    lgdt [gdt_descriptor]               ; load GDT
    mov eax, cr0
    or eax, CR0_PE                      ; set PE bit
    mov cr0, eax

    jmp CODE_SEG:pm_entry


; Global Descriptor Table
gdt_start:

    gdt_null:
        dd 0x0
        dd 0x0
    
    gdt_code:
        dw 0xffff               ; limit (low)
        dw 0x0                  ; base (low)
        db 0x0                  ; base (mid)
        db 0b10011010           ; access
        db 0b11001111           ; flags
        db 0x0                  ; base (high)
    
    gdt_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; size of GDT
    dd gdt_start                ; address of GDT


; Protected mode entry
[bits 32]
pm_entry:
    mov ax, DATA_SEG        ; load data segment selector
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov esp, 0x90000        ; setup stack base

    jmp BOOTENV_LOCATION

times 8192 - ($ - $$) db 0
