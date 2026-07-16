#include "arch/interrupts/idt.h"

#include "utils/types.h"

#define ISR_DECLARE(n) extern void i686_ISR##n(void)
#define IRQ_DECLARE(n) extern void i686_IRQ##n(void)

/* ISR 0-31 */
ISR_DECLARE(0);
ISR_DECLARE(1);
ISR_DECLARE(2);
ISR_DECLARE(3);
ISR_DECLARE(4);
ISR_DECLARE(5);
ISR_DECLARE(6);
ISR_DECLARE(7);
ISR_DECLARE(8);
ISR_DECLARE(9);
ISR_DECLARE(10);
ISR_DECLARE(11);
ISR_DECLARE(12);
ISR_DECLARE(13);
ISR_DECLARE(14);
ISR_DECLARE(15);
ISR_DECLARE(16);
ISR_DECLARE(17);
ISR_DECLARE(18);
ISR_DECLARE(19);
ISR_DECLARE(20);
ISR_DECLARE(21);
ISR_DECLARE(22);
ISR_DECLARE(23);
ISR_DECLARE(24);
ISR_DECLARE(25);
ISR_DECLARE(26);
ISR_DECLARE(27);
ISR_DECLARE(28);
ISR_DECLARE(29);
ISR_DECLARE(30);
ISR_DECLARE(31);

/* IRQ 0-15 */
IRQ_DECLARE(0);
IRQ_DECLARE(1);
IRQ_DECLARE(2);
IRQ_DECLARE(3);
IRQ_DECLARE(4);
IRQ_DECLARE(5);
IRQ_DECLARE(6);
IRQ_DECLARE(7);
IRQ_DECLARE(8);
IRQ_DECLARE(9);
IRQ_DECLARE(10);
IRQ_DECLARE(11);
IRQ_DECLARE(12);
IRQ_DECLARE(13);
IRQ_DECLARE(14);
IRQ_DECLARE(15);

struct [[gnu::packed]] idt_entry {
    u16 offset_low;
    u16 selector;
    u8 zero;
    u8 type_attr;
    u16 offset_high;
};

struct [[gnu::packed]] idt_pointer {
    u16 limit;
    u32 base;
};

constexpr u32 IDT_ENTRIES = 256u;
constexpr u32 ISR_COUNT = 32u;
constexpr u32 IRQ_COUNT = 16u;

constexpr u8 IRQ_BASE_VECTOR = 0x20u;

constexpr u16 KERNEL_CODE_SELECTOR = 0x08u;
constexpr u8 IDT_INTERRUPT_GATE = 0x8Eu;

constexpr u16 IDT_OFFSET_MASK = 0xFFFFu;

static void (*isr_table[ISR_COUNT])(void) = {
    i686_ISR0,  i686_ISR1,  i686_ISR2,  i686_ISR3,  i686_ISR4,  i686_ISR5,
    i686_ISR6,  i686_ISR7,  i686_ISR8,  i686_ISR9,  i686_ISR10, i686_ISR11,
    i686_ISR12, i686_ISR13, i686_ISR14, i686_ISR15, i686_ISR16, i686_ISR17,
    i686_ISR18, i686_ISR19, i686_ISR20, i686_ISR21, i686_ISR22, i686_ISR23,
    i686_ISR24, i686_ISR25, i686_ISR26, i686_ISR27, i686_ISR28, i686_ISR29,
    i686_ISR30, i686_ISR31
};

static void (*irq_table[IRQ_COUNT])(void) = {
    i686_IRQ0,  i686_IRQ1,  i686_IRQ2,  i686_IRQ3, i686_IRQ4,  i686_IRQ5,
    i686_IRQ6,  i686_IRQ7,  i686_IRQ8,  i686_IRQ9, i686_IRQ10, i686_IRQ11,
    i686_IRQ12, i686_IRQ13, i686_IRQ14, i686_IRQ15
};

struct idt_entry alignas(16) idt[IDT_ENTRIES];
struct idt_pointer alignas(16) idt_p;

static inline void set_idt_entry(u8 vector,
                                 u32 handler,
                                 u16 selector,
                                 u8 attributes)
{
    idt[vector].offset_low = handler & IDT_OFFSET_MASK;
    idt[vector].selector = selector;
    idt[vector].zero = 0;
    idt[vector].type_attr = attributes;
    idt[vector].offset_high = (handler >> 16) & IDT_OFFSET_MASK;
}

void idt_init(void)
{
    idt_p.limit = sizeof(idt) - 1;
    idt_p.base = (u32)&idt;

    // Clear IDT
    for (u32 i = 0; i < IDT_ENTRIES; i++) {
        idt[i].offset_low = 0;
        idt[i].selector = 0;
        idt[i].zero = 0;
        idt[i].type_attr = 0;
        idt[i].offset_high = 0;
    }

    // Install ISR 0-31
    for (u32 i = 0; i < ISR_COUNT; i++) {
        set_idt_entry(
            i, (u32)isr_table[i], KERNEL_CODE_SELECTOR, IDT_INTERRUPT_GATE);
    }

    // Install IRQ 32-47
    for (u32 i = 0; i < IRQ_COUNT; i++) {
        set_idt_entry(IRQ_BASE_VECTOR + i,
                      (u32)irq_table[i],
                      KERNEL_CODE_SELECTOR,
                      IDT_INTERRUPT_GATE);
    }

    __asm__ volatile("lidt (%0)" : : "r"(&idt_p));
}
