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
                                ; dl already with the correct value
    int 0x13                    ; BIOS disk interrupt
    jc disk_error               ; Jump if carry flag set (error)
    ret                         ; Return if success

disk_error:
    mov si, msg_disk_error      ; Load error message
    call print_string           ; Print error
    hlt                         ; Halt CPU
    jmp $                       ; Infinite loop


