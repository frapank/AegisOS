bits 32

section .text
extern i686_ISR_Handler

%macro isr_noerror 1
global i686_ISR%1
i686_ISR%1:
    cli
    push byte 0
    push byte %1
    call isr_common
%endmacro

%macro isr_error 1
global i686_ISR%1
i686_ISR%1:
    cli
    push byte %1
    call isr_common
%endmacro

isr_noerror 0  ; 0  - Divide Error
isr_noerror 1  ; 1  - Debug
isr_noerror 2  ; 2  - Non Maskable Interrupt
isr_noerror 3  ; 3  - Breakpoint
isr_noerror 4  ; 4  - Overflow
isr_noerror 5  ; 5  - BOUND Range Exceeded
isr_noerror 6  ; 6  - Invalid Opcode
isr_noerror 7  ; 7  - Device Not Available
isr_error 8    ; 8  - Double Fault
isr_noerror 9  ; 9  - Coprocessor Segment Overrun
isr_error 10   ; 10 - Invalid TSS
isr_error 11   ; 11 - Segment Not Present
isr_error 12   ; 12 - Stack-Segment Fault
isr_error 13   ; 13 - General Protection Fault
isr_error 14   ; 14 - Page Fault
isr_noerror 15 ; 15 - (Reserved)
isr_noerror 16 ; 16 - x87 Floating-Point Exception
isr_noerror 17 ; 17 - Alignment Check
isr_noerror 18 ; 18 - Machine Check
isr_noerror 19 ; 19 - SIMD Floating-Point Exception
isr_noerror 20 ; 20 - Virtualization Exception

; 21..31 - reserved 
isr_noerror 21
isr_noerror 22
isr_noerror 23
isr_noerror 24
isr_noerror 25
isr_noerror 26
isr_noerror 27
isr_noerror 28
isr_noerror 29
isr_noerror 30
isr_noerror 31

isr_common:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    jmp i686_ISR_Handler
    
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    
    popa
    add esp, 8
    sti
    iret
