[bits 16]
org 0x1000

second_stage:

    mov si, msg_loaded        ; Load pointer to "Second Stage loaded" message
    call print_string         ; Print message
    mov si, msg_wait          ; Load pointer to "Press a key" message
    call print_string         ; Print message
    ;call wait_key            ; Wait for key press

    call read_disk            ; Read kernel from disk
    call start_protected      ; Switch to protected mode

;----------------
; Data
msg_loaded          db "[+] Second Stage loaded!",0Dh,0Ah,0
msg_wait            db "[*] Press a key to continue...",0Dh,0Ah,0
msg_disk_error      db "[-] Error while reading the disk",0Dh,0Ah,0
msg_done            db "[+] Done", 0Dh, 0Ah,0

CODE_SEG            equ gdt_code - gdt_start                   ; Offset of code segment in GDT
DATA_SEG            equ gdt_data - gdt_start                   ; Offset of data segment in GDT
KERNEL_LOCATION     equ 0x2000                                 ; Load address of kernel
KERNEL_SECTORS      equ 10                                     ; Number of sectors to read
KERNEL_START_SECTOR equ 3                                      ; Start sector on disk

;----------------
; Graphics
print_string:
    mov ah, 0x0e            ; BIOS display function
    .print_loop:
        lodsb               ; Increment SI to point to next character and
                            ; load character in al
        cmp al, 0           ; Check for null terminator (end of string)
        je .print_done      ; Jump if end of string
        int 0x10            ; Call BIOS video interrupt
        jmp .print_loop     ; Repeat loop for next character
    .print_done:
        ret

clear_screen:
    mov ah, 0x06            ; Scroll window function
    xor al, al              ; Clear lines to scroll
    mov bh, 0x07            ; Attribute for blank lines
    xor cx, cx              ; Upper left corner (row/col)
    mov dh, 24-1            ; Lower right row
    mov dl, 80-1            ; Lower right column
    int 0x10                ; BIOS video interrupt to scroll
    
    mov ah, 0x02            ; Set cursor position
    xor bh, bh              ; Page number
    xor dh, dh              ; Row = 0
    xor dl, dl              ; Column = 0
    int 0x10                ; BIOS interrupt to set cursor
    ret

wait_key:
    sti                     ; Enable interrupts
    mov ah, 0x00            ; BIOS keyboard input function
    int 0x16                ; Wait for keypress
    cli                     ; Disable interrupts again
    ret                     ; Return from function

;----------------
; Disk operations
read_disk:
    cli                         ; Disable interrupts
    xor ax, ax                  ; Clear AX
    mov ds, ax                  ; Set DS to 0
    mov es, ax                  ; Set ES to 0
    mov bx, KERNEL_LOCATION     ; Destination address for disk read

    mov ah, 0x02                ; BIOS read sectors function
    mov al, KERNEL_SECTORS      ; Number of sectors to read
    mov ch, 0x00                ; Cylinder 0
    mov cl, KERNEL_START_SECTOR ; Start sector
    mov dh, 0x00                ; Head 0
    mov dl, 0x80                ; Drive number (0x80 = first HDD) ( should change )
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

