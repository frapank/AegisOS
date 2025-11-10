[bits 16]
org 0x7c00

start:

    cli                     ; Disable interrupts
    ; Set Stack
    mov ax, 0x9000          ; Load stack segment address into AX
    mov ss, ax              ; Set SS (Stack Segment)
    mov sp, 0xFFFF          ; Set SP to top of stack
    mov bp, sp              ; Set BP for stack frame usage

    ; Set Data Segment
    mov ax, 0x0000
    mov ds, ax              ; Set Dat Segment (to zero)
    mov es, ax              ; Set Extra Segment (to zero)

    ; Set boot disk
    mov [boot_disk], dl     ; Store boot drive into boot_disk (used in read_disk)

    call clear_screen

    ; Load segment and load second mode
    call read_disk
    call start_second

;----------------
; Data
msg_disk_error      db "[-] Can't find second stage, stopped" ,0Dh,0Ah,0

boot_disk           db  0         ; Store boot drive number
STAGE_LOCATION      equ 0x1000    ; Load address for second stage
STAGE_SECTORS       equ 2         ; Number of sectors to read
STAGE_START_SECTOR  equ 2         ; First sector of second stage

%include "boot/graphics.asm" ; Include (copy) print_string, clear_screen and wait_key

;------------------
; Setup Protected
read_disk:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov bx, STAGE_LOCATION      ; Destination address for disk read

    mov ah, 0x02                ; BIOS read sectors function
    mov al, STAGE_SECTORS       ; Number of sectors to read
    mov ch, 0x00                ; Cylinder 0
    mov cl, STAGE_START_SECTOR  ; Starting sector
    mov dh, 0x00                ; Head 0
    mov dl, [boot_disk]         ; Drive number
    int 0x13                    ; BIOS disk interrupt
    jc disk_error               ; Jump if carry flag set (error)
    ret                         ; Return if successful

disk_error:
    mov si, msg_disk_error      ; Load error messag
    call print_string           ; Print error message
    hlt                         ; Halt CPU
    jmp $                       ; Infinite loop

start_second:
    jmp STAGE_LOCATION          ; Jump to second stage

times 510-($-$$) db 0           ; Fill up to 510 bytes with zeros
dw 0xaa55                       ; Boot signature

