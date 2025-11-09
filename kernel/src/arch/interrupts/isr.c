#include "arch/interrupts/isr.h"
#include "core/print.h"
#include "utils/defhelp.h"

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

#define DECLARE_ISR_WEAK(n) \
void isr##n##_man(struct regs *r) __attribute__((weak, alias("default_isr_handler")))

DECLARE_ISR_WEAK(0);
DECLARE_ISR_WEAK(1);
DECLARE_ISR_WEAK(2);
DECLARE_ISR_WEAK(3);
DECLARE_ISR_WEAK(4);
DECLARE_ISR_WEAK(5);
DECLARE_ISR_WEAK(6);
DECLARE_ISR_WEAK(7);
DECLARE_ISR_WEAK(8);
DECLARE_ISR_WEAK(9);
DECLARE_ISR_WEAK(10);
DECLARE_ISR_WEAK(11);
DECLARE_ISR_WEAK(12);
DECLARE_ISR_WEAK(13);
DECLARE_ISR_WEAK(14);
DECLARE_ISR_WEAK(15);
DECLARE_ISR_WEAK(16);
DECLARE_ISR_WEAK(17);
DECLARE_ISR_WEAK(18);
DECLARE_ISR_WEAK(19);
DECLARE_ISR_WEAK(20);
DECLARE_ISR_WEAK(21);
DECLARE_ISR_WEAK(22);
DECLARE_ISR_WEAK(23);
DECLARE_ISR_WEAK(24);
DECLARE_ISR_WEAK(25);
DECLARE_ISR_WEAK(26);
DECLARE_ISR_WEAK(27);
DECLARE_ISR_WEAK(28);
DECLARE_ISR_WEAK(29);
DECLARE_ISR_WEAK(30);
DECLARE_ISR_WEAK(31);

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

static void __attribute__((noreturn)) default_isr_handler(struct regs* regs){
    printk_color("Unhandled isr %s...\n", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, exception_names[regs->int_no]);
    for (;;) { asm volatile("hlt"); } // Todo: Remove when panic are added
}

void __attribute__((cdecl)) i686_ISR_Handler(struct regs* regs) {
    uint8_t num = regs->int_no;
    if (likely(num < 32)) {
        isr_handlers[num](regs);
    } else {
        printk_color("Unknown interrupt number: %d\n", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, num);
        for (;;) { asm volatile("hlt"); } // Todo: Remove when panic are added
    }
}

