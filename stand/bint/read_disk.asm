; Setup Protected
read_disk:
    push bp
    mov bp, sp

    mov si, [bp+4]              ; DAP
    mov dl, [bp+6]              ; Boot disk

    mov ah, 0x42                ; extended read 
    int 0x13 

    pop bp
    ret


