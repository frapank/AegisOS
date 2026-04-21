bits 32

section .text
extern i686_ISR_Handler 
extern i686_IRQ_Handler

; --- ISR ---
%macro isr_noerror 1
global i686_ISR%1
i686_ISR%1:
    cli
    push dword 0
    push dword %1
    jmp isr_common
%endmacro

%macro isr_error 1
global i686_ISR%1
i686_ISR%1:
    cli
    push dword %1
    jmp isr_common
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

; --- IRQ ---
%macro irq_noerror 1
global i686_IRQ%1
i686_IRQ%1:
    cli
    push dword 0
    push dword (32 + %1)
    jmp irq_common
%endmacro

irq_noerror 0  ; IRQ0  - timer (PIT)
irq_noerror 1  ; IRQ1  - keyboard
irq_noerror 2  ; IRQ2  - cascade
irq_noerror 3
irq_noerror 4
irq_noerror 5
irq_noerror 6
irq_noerror 7
irq_noerror 8  ; IRQ8  - rtc (onboard)
irq_noerror 9
irq_noerror 10
irq_noerror 11
irq_noerror 12 ; PS/2 mouse
irq_noerror 13
irq_noerror 14 ; primary ATA
irq_noerror 15 ; secondary ATA

isr_common:
    pusha
    mov eax, ds
    push eax
    mov eax, es
    push eax
    mov eax, fs
    push eax
    mov eax, gs
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp
    push eax
    call i686_ISR_Handler
    add esp, 4

    pop eax
    mov gs, ax
    pop eax
    mov fs, ax
    pop eax
    mov es, ax
    pop eax
    mov ds, ax

    popa

    add esp, 8
    iret

irq_common:
    pusha

    mov eax, ds
    push eax
    mov eax, es
    push eax
    mov eax, fs
    push eax
    mov eax, gs
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp
    push eax
    call i686_IRQ_Handler
    add esp, 4

    mov ebx, [esp + 48]    ; ebx = int_no
    cmp ebx, 0x28          ; if >= 0x28 (40) is IRQ del slave (remapped base 0x20)
    jb .send_master_only

    mov al, 0x20
    out 0xA0, al
.send_master_only:
    mov al, 0x20
    out 0x20, al

    pop eax
    mov gs, ax
    pop eax
    mov fs, ax
    pop eax
    mov es, ax
    pop eax
    mov ds, ax

    popa
    add esp, 8

    iret

