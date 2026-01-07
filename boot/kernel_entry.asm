[bits 32]

section .bss
align 4096
page_dir resd 1024
align 4096
page_table_0 resd 1024

magic_number resd 1
flags resd 1

global _start
extern kernel_main

section .text
_start:
    cli

    mov [magic_number], eax
    mov [flags], ebx

    call setup_paging
    call load_page_dir
    call enable_paging

    mov eax, [magic_number]
    mov ebx, [flags]

    push ebx            ; Pass boot_info
    push eax            ; Pass magic_number
    call kernel_main

.halt:
    hlt
    jmp .halt

; ----------------------
; Virtual memory
setup_paging:
    mov edi, page_table_0
    xor eax, eax
    mov ecx, 1024

.fill:
    mov ebx, eax
    or ebx, 0x3
    mov [edi], ebx
    add eax, 4096
    add edi, 4
    dec ecx
    jnz .fill

    mov eax, page_table_0
    or eax, 0x3
    mov [page_dir], eax
    ret

load_page_dir:
    mov eax, page_dir
    mov cr3, eax
    ret

enable_paging:
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ret


