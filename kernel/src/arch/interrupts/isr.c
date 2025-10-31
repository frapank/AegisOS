#include "arch/interrupts/isr.h"
#include "core/print.h"
#include "drivers/vga.h"

char* exception_names[32] = {
    "0: Zero for zero division",
    "1: Debug",
    "2: Non maskable interrupt (NMI)",
    "3: Breakpoint",
    "4: Overflow",
    "5: BOUND Range Exceeded",
    "6: Invalid Opcode",
    "7: Device Not Available (No FPU)",
    "8: Double Fault",
    "9: Coprocessor Segment Overrun",
    "10: Invalid TSS",
    "11: Segment Not Present",
    "12: Stack-Segment Fault",
    "13: General Protection Fault",
    "14: Page Fault",
    "15: (Reserved)",
    "16: x87 FPU Floating-Point Error",
    "17: Alignment Check",
    "18: Machine Check",
    "19: SIMD Floating-Point Exception",
    "20: Virtualization Exception",
    "21: Reserved",
    "22: Reserved",
    "23: Reserved",
    "24: Reserved",
    "25: Reserved",
    "26: Reserved",
    "27: Reserved",
    "28: Reserved",
    "29: Reserved",
    "30: Reserved",
    "31: Reserved"
};

void __attribute__((cdecl)) i686_ISR_Handler(struct regs* regs) {
    print_color("Problem isr: %s\n", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, exception_names[regs->int_no]);
    for (;;);
}

