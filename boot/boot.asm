[bits 16]
org 0x7c00

start:

    cli
    ; Set Stack
    mov ax, 0x9000
    mov ss, ax
    mov sp, 0xFFFF
    mov bp, sp

    ; Set Data Segment
    mov ax, 0x0000
    mov ds, ax
    mov es, ax

    ; Set boot disk
    mov [boot_disk], dl

    ; Render
    call clear_screen
    mov si, msg_wait
    call print_string

    call read_disk
    call start_second
    sti

;----------------
; Data
msg_wait            db "[*] Loading second stage...",0Dh,0Ah,0
msg_disk_error      db "[-] Can't find second stage, stopped",0Dh,0Ah,0

boot_disk           db 0
STAGE_LOCATION      equ 0x1000
STAGE_SECTORS       equ 1
STAGE_START_SECTOR  equ 2

;----------------
; Graphics
print_string:
    mov ah, 0x0e
    .print_loop:
        lodsb
        cmp al, 0 
        je .print_done
        int 0x10
        jmp .print_loop
    .print_done:
        ret

clear_screen:
    mov ah, 0x06
    xor al, al
    mov bh, 0x07
    xor cx, cx
    mov dh, 24-1
    mov dl, 80-1
    int 0x10
    
    mov ah, 0x02
    xor bh, bh
    xor dh, dh
    xor dl, dl
    int 0x10
    ret

;------------------
; Setup Protected
read_disk:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax 
    mov bx, STAGE_LOCATION

    mov ah, 0x02
    mov al, STAGE_SECTORS
    mov ch, 0x00
    mov cl, STAGE_START_SECTOR
    mov dh, 0x00
    mov dl, [boot_disk]
    int 0x13
    jc disk_error
    ret
disk_error:
    mov si, msg_disk_error
    call print_string
    hlt
    jmp $

start_second:
    jmp 0x0000:STAGE_LOCATION

times 510-($-$$) db 0
dw 0xaa55
