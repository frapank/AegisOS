#include "arch/interrupts/isr.h"
#include "core/panic.h"
#include "core/print_vga_text.h"
#include "utils/defhelp.h"
#include "utils/types.h"

constexpr u8 CPU_EXCEPTION_COUNT = 32u;

static const char* const exception_names[CPU_EXCEPTION_COUNT] = {
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
    "15: Reserved",
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

#define DECLARE_DEFAULT_ISR(n)                                                 \
    [[noreturn, gnu::alias("default_isr_handler")]]                            \
    void isr##n##_man(struct regs* r)

DECLARE_DEFAULT_ISR(0);
DECLARE_DEFAULT_ISR(1);
DECLARE_DEFAULT_ISR(2);
DECLARE_DEFAULT_ISR(3);
DECLARE_DEFAULT_ISR(4);
DECLARE_DEFAULT_ISR(5);
DECLARE_DEFAULT_ISR(6);
DECLARE_DEFAULT_ISR(7);
DECLARE_DEFAULT_ISR(8);
DECLARE_DEFAULT_ISR(9);
DECLARE_DEFAULT_ISR(10);
DECLARE_DEFAULT_ISR(11);
DECLARE_DEFAULT_ISR(12);
DECLARE_DEFAULT_ISR(13);
DECLARE_DEFAULT_ISR(14);
DECLARE_DEFAULT_ISR(15);
DECLARE_DEFAULT_ISR(16);
DECLARE_DEFAULT_ISR(17);
DECLARE_DEFAULT_ISR(18);
DECLARE_DEFAULT_ISR(19);
DECLARE_DEFAULT_ISR(20);
DECLARE_DEFAULT_ISR(21);
DECLARE_DEFAULT_ISR(22);
DECLARE_DEFAULT_ISR(23);
DECLARE_DEFAULT_ISR(24);
DECLARE_DEFAULT_ISR(25);
DECLARE_DEFAULT_ISR(26);
DECLARE_DEFAULT_ISR(27);
DECLARE_DEFAULT_ISR(28);
DECLARE_DEFAULT_ISR(29);
DECLARE_DEFAULT_ISR(30);
DECLARE_DEFAULT_ISR(31);

static void (*const isr_handlers[CPU_EXCEPTION_COUNT])(struct regs*) = {
    isr0_man,  isr1_man,  isr2_man,  isr3_man,  isr4_man,  isr5_man,  isr6_man,
    isr7_man,  isr8_man,  isr9_man,  isr10_man, isr11_man, isr12_man, isr13_man,
    isr14_man, isr15_man, isr16_man, isr17_man, isr18_man, isr19_man, isr20_man,
    isr21_man, isr22_man, isr23_man, isr24_man, isr25_man, isr26_man, isr27_man,
    isr28_man, isr29_man, isr30_man, isr31_man
};

[[noreturn]]
static void default_isr_handler(struct regs* regs)
{
    panick(exception_names[regs->int_no]);

    for (;;)
        ;
}

[[gnu::cdecl]] void i686_ISR_Handler(struct regs* regs)
{
    const u8 num = regs->int_no;

    if (likely(num < CPU_EXCEPTION_COUNT)) {
        isr_handlers[num](regs);
    } else {
        panick("Invalid interrupt number\n");
    }
}
