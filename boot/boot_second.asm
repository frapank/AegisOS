[bits 16]
org 0x1000

second_stage:


    ; Print welcome ascii 
    ; mov si, ascii_n = argument
    mov si, ascii_1
    call print_string
    mov si, ascii_2
    call print_string
    mov si, ascii_3
    call print_string
    mov si, ascii_4
    call print_string
    mov si, ascii_5
    call print_string
    mov si, ascii_6
    call print_string
    mov si, ascii_7
    call print_string
    mov si, ascii_8
    call print_string
    mov si, ascii_9
    call print_string

    call wait_key                   ; Wait for key press

    mov dl, [0x7E00]                ; Get the saved boot_sector

    call read_disk                  ; Read kernel from disk
    call start_protected            ; Switch to protected mode

;----------------
; Data
msg_disk_error      db "[-] Error while reading the disk",0Dh,0Ah,0

ascii_1             db "+---------------------------------+",0Dh,0Ah,0
ascii_2             db "|         LAIN BOOTLOADER         |",0Dh,0Ah,0
ascii_3             db "|                                 |",0Dh,0Ah,0
ascii_4             db "|  Welcome to the official LainOS |",0Dh,0Ah,0
ascii_5             db "|            bootloader           |",0Dh,0Ah,0
ascii_6             db "|                                 |",0Dh,0Ah,0
ascii_7             db "|      Press a key to continue    |",0Dh,0Ah,0
ascii_8             db "|                                 |",0Dh,0Ah,0
ascii_9             db "+---------------------------------+",0Dh,0Ah,0

CODE_SEG            equ gdt_code - gdt_start                   ; Offset of code segment in GDT
DATA_SEG            equ gdt_data - gdt_start                   ; Offset of data segment in GDT
KERNEL_LOCATION     equ 0x2000                                 ; Load address of kernel
KERNEL_SECTORS      equ 13                                     ; Number of sectors to read
KERNEL_START_SECTOR equ 4                                      ; Start sector on disk

%include "boot/graphics.asm" ; Include (copy) print_string, clear_screen and wait_key

;----------------
; Disk operations
read_disk:
    cli
    xor ax, ax
    mov ds, ax                  ; Set DS to 0
    mov es, ax                  ; Set ES to 0
    mov bx, KERNEL_LOCATION     ; Destination address for disk read

    mov ah, 0x02                ; BIOS read sectors function
    mov al, KERNEL_SECTORS      ; Number of sectors to read
    mov ch, 0x00                ; Cylinder 0
    mov cl, KERNEL_START_SECTOR ; Start sector
    mov dh, 0x00                ; Head 0
    int 0x13                    ; BIOS disk interrupt
    jc disk_error               ; Jump if carry flag set (error)
    ret                         ; Return if success

disk_error:
    mov si, msg_disk_error      ; Load error message
    call print_string           ; Print error
    hlt                         ; Halt CPU
    jmp $                       ; Infinite loop

;----------------
; Protected mode setup
start_protected:
    cli                                 ; Disable interrupts
    xor ax, ax
    mov ds, ax                          ; DS = 0
    mov es, ax                          ; ES = 0
    lgdt [gdt_descriptor]               ; Load GDT
    mov eax, cr0
    or eax, 1                           ; Set PE bit to enable protected mode
    mov cr0, eax
    jmp CODE_SEG:start_protected_mode   ; Far jump to flush prefetch queue

;----------------
; Global Descriptor Table
gdt_start:

gdt_null:
    dd 0x0
    dd 0x0

gdt_code:
    dw 0xffff               ; Limit low
    dw 0x0                  ; Base low
    db 0x0                  ; Base middle
    db 0b10011010           ; Access byte (code segment)
    db 0b11001111           ; Flags
    db 0x0                  ; Base high

gdt_data:
    dw 0xffff               ; Limit low
    dw 0x0                  ; Base low
    db 0x0                  ; Base middle
    db 0b10010010           ; Access byte (data segment)
    db 0b11001111           ; Flags
    db 0x0                  ; Base high

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size of GDT
    dd gdt_start                ; Address of GDT

;----------------
; Protected mode entry
[bits 32]
start_protected_mode:

    mov ax, DATA_SEG        ; Load data segment selector
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000        ; Setup stack base
    mov esp, ebp            ; Initialize stack pointer

    jmp KERNEL_LOCATION     ; Jump to loaded kernel

times 1024 - ($ - $$) db 0
