#include "arch/interrupts/isr.h"
#include "core/print.h"
#include "core/panic.h"
#include "utils/defhelp.h"
#include "utils/types.h"

static char* const exception_names[32] = {
    "0: Division for zero",
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

#define UNDECLARED_ISR(n) \
void isr##n##_man(struct regs *r) __attribute__((alias("default_isr_handler")))

UNDECLARED_ISR(0);
UNDECLARED_ISR(1);
UNDECLARED_ISR(2);
UNDECLARED_ISR(3);
UNDECLARED_ISR(4);
UNDECLARED_ISR(5);
UNDECLARED_ISR(6);
UNDECLARED_ISR(7);
UNDECLARED_ISR(8);
UNDECLARED_ISR(9);
UNDECLARED_ISR(10);
UNDECLARED_ISR(11);
UNDECLARED_ISR(12);
UNDECLARED_ISR(13);
UNDECLARED_ISR(14);
UNDECLARED_ISR(15);
UNDECLARED_ISR(16);
UNDECLARED_ISR(17);
UNDECLARED_ISR(18);
UNDECLARED_ISR(19);
UNDECLARED_ISR(20);
UNDECLARED_ISR(21);
UNDECLARED_ISR(22);
UNDECLARED_ISR(23);
UNDECLARED_ISR(24);
UNDECLARED_ISR(25);
UNDECLARED_ISR(26);
UNDECLARED_ISR(27);
UNDECLARED_ISR(28);
UNDECLARED_ISR(29);
UNDECLARED_ISR(30);
UNDECLARED_ISR(31);

static void (* const isr_handlers[32])(struct regs*) = {
    isr0_man,  isr1_man,  isr2_man,  isr3_man,
    isr4_man,  isr5_man,  isr6_man,  isr7_man,
    isr8_man,  isr9_man,  isr10_man, isr11_man,
    isr12_man, isr13_man, isr14_man, isr15_man,
    isr16_man, isr17_man, isr18_man, isr19_man,
    isr20_man, isr21_man, isr22_man, isr23_man,
    isr24_man, isr25_man, isr26_man, isr27_man,
    isr28_man, isr29_man, isr30_man, isr31_man
};

static void __attribute__((noreturn)) default_isr_handler(struct regs* regs)
{
    printk_color("Unhandled isr %s...\n", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, exception_names[regs->int_no]);
    panick("");
}

void __attribute__((cdecl)) i686_ISR_Handler(struct regs* regs) 
{
    u8 num = regs->int_no;

    if (likely(num < 32)) {
        isr_handlers[num](regs);
    } else {
        printk_color("Unknown interrupt number: %d\n", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, num);
        for (;;) { asm volatile("hlt"); } // Todo: Remove when panic are added
    }
}

