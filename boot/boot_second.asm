[bits 16]
org 0x1000

second_stage:

    mov si, msg_loaded
    call print_string
    mov si, msg_wait
    call print_string
    ;call wait_key

    call read_disk
    call start_protected

;----------------
; Data
msg_loaded          db "[+] Second Stage loaded!",0Dh,0Ah,0
msg_wait            db "[*] Press a key to continue...",0Dh,0Ah,0
msg_disk_error      db "[-] Error while reading the disk",0Dh,0Ah,0
msg_done            db "[+] Done", 0Dh, 0Ah,0
msg_t db "&", 0Dh, 0Ah,0

CODE_SEG            equ gdt_code - gdt_start
DATA_SEG            equ gdt_data - gdt_start
KERNEL_LOCATION     equ 0x2000
KERNEL_SECTORS      equ 10
KERNEL_START_SECTOR equ 3

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

wait_key:
    sti
    mov ah, 0x00
    int 0x16
    cli
    ret

read_disk:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax 
    mov bx, KERNEL_LOCATION

    mov ah, 0x02
    mov al, KERNEL_SECTORS
    mov ch, 0x00
    mov cl, KERNEL_START_SECTOR
    mov dh, 0x00
    mov dl, 0x80 ; debug
    int 0x13
    jc disk_error
    ret
disk_error:
    mov si, msg_disk_error
    call print_string
    hlt
    jmp $

;----------------
; Protected mode
start_protected:
    cli
    xor ax, ax
    mov ds, ax       ; DS = 0
    mov es, ax       ; ES = 0
    lgdt [gdt_descriptor]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEG:start_protected_mode

gdt_start:
    gdt_null:
        dd 0x0
        dd 0x0

    gdt_code:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10011010
        db 0b11001111
        db 0x0

    gdt_data:
        dw 0xffff
        dw 0x0
        db 0x0
        db 0b10010010
        db 0b11001111
        db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

[bits 32]
start_protected_mode:

    mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x90000
	mov esp, ebp

    jmp KERNEL_LOCATION
