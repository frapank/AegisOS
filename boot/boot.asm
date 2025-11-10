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
    mov ax, 0x0000          ; Clear AX to 0
    mov ds, ax              ; Set Dat Segment (to zero)
    mov es, ax              ; Set Extra Segment (to zero)

    ; Set boot disk
    mov [boot_disk], dl     ; Store boot drive into boot_disk (used in read_disk)

    ; Render
    call clear_screen       ; Clear the screen
    mov si, msg_wait        ; Load 'argument' for print_string
    call print_string       ; Call print 

    call read_disk          ; Load second stage from disk
    call start_second       ; Jump to second stage

;----------------
; Data
msg_wait            db "[*] Loading second stage..."          ,0Dh,0Ah,0
msg_disk_error      db "[-] Can't find second stage, stopped" ,0Dh,0Ah,0

boot_disk           db  0         ; Store boot drive number
STAGE_LOCATION      equ 0x1000    ; Load address for second stage
STAGE_SECTORS       equ 1         ; Number of sectors to read
STAGE_START_SECTOR  equ 2         ; First sector of second stage

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


;------------------
; Setup Protected
read_disk:
    cli                         ; Disable interrupts during disk read
    xor ax, ax                  ; Clear AX
    mov ds, ax                  ; Set DS to 0
    mov es, ax                  ; Set ES to 0
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
    mov si, msg_disk_error      ; Load error message pointer
    call print_string           ; Print error message
    hlt                         ; Halt CPU
    jmp $                       ; Infinite loop

start_second:
    jmp STAGE_LOCATION          ; Jump to second stage

times 510-($-$$) db 0           ; Fill up to 510 bytes with zeros
dw 0xaa55                       ; Boot signature

