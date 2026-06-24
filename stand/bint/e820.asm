E820_DATA    equ 0x0500         ; 4 byte count + 12 * 24 byte entries
E820_SMAP    equ 0x534D4150     ; 'SMAP'
E820_MAX     equ 12

; Detect E820 memory map, store at E820_DATA
e820_detect:
    pusha
    push es

    xor ax, ax
    mov es, ax
    mov di, E820_DATA + 4           ; entries start after count u32
    mov dword [E820_DATA], 0        ; count = 0
    xor ebx, ebx                    ; continuation = 0

.loop:
    mov eax, 0xE820
    mov ecx, 24
    mov edx, E820_SMAP
    int 0x15
    jc .done                        ; error or and list
    cmp eax, E820_SMAP
    jne .done
    cmp ecx, 20                     ; valid entry
    jl .next

    inc dword [E820_DATA]           ; count++
    add di, 24

    cmp dword [E820_DATA], E820_MAX
    jge .done                       ; buffer full 

.next:
    test ebx, ebx                   ; last entry
    jnz .loop

.done:
    pop es
    popa
    ret
