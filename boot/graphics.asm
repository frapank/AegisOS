;--------------------
; Print a string
; Args:
; si = string to print
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

;--------------------
; Print the welcome screen
print_welcome:
    mov si, line_down
    call print_string
    mov si, lain_line1
    call print_string
    mov si, lain_line2
    call print_string
    mov si, lain_line3
    call print_string
    mov si, lain_line4
    call print_string
    mov si, lain_line5
    call print_string
    mov si, line_down
    call print_string
    mov si, msg
    call print_string
    mov si, msg_wait
    call print_string
    ret

;--------------------
; Clear screen
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

;--------------------
; Disable blink cursor
disable_cursor:
    mov ah, 0x01 
    mov ch, 0x20
    mov cl, 0x00
    int 0x10

;--------------------
; Wait for an input
wait_key:
    mov ah, 0x00
    int 0x16
    ret

