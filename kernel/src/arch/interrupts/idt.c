#include "arch/interrupts/idt.h"

#define ISR_DECLARE(n) extern void i686_ISR##n(void)
#define IRQ_DECLARE(n) extern void i686_IRQ##n(void)

/* ISR 0-31 */
ISR_DECLARE(0);  ISR_DECLARE(1);  ISR_DECLARE(2);  ISR_DECLARE(3);
ISR_DECLARE(4);  ISR_DECLARE(5);  ISR_DECLARE(6);  ISR_DECLARE(7);
ISR_DECLARE(8);  ISR_DECLARE(9);  ISR_DECLARE(10); ISR_DECLARE(11);
ISR_DECLARE(12); ISR_DECLARE(13); ISR_DECLARE(14); ISR_DECLARE(15);
ISR_DECLARE(16); ISR_DECLARE(17); ISR_DECLARE(18); ISR_DECLARE(19);
ISR_DECLARE(20); ISR_DECLARE(21); ISR_DECLARE(22); ISR_DECLARE(23);
ISR_DECLARE(24); ISR_DECLARE(25); ISR_DECLARE(26); ISR_DECLARE(27);
ISR_DECLARE(28); ISR_DECLARE(29); ISR_DECLARE(30); ISR_DECLARE(31);

/* IRQ 0-15 */
IRQ_DECLARE(0); IRQ_DECLARE(1); IRQ_DECLARE(2); IRQ_DECLARE(3);
IRQ_DECLARE(4); IRQ_DECLARE(5); IRQ_DECLARE(6); IRQ_DECLARE(7);
IRQ_DECLARE(8); IRQ_DECLARE(9); IRQ_DECLARE(10); IRQ_DECLARE(11);
IRQ_DECLARE(12); IRQ_DECLARE(13); IRQ_DECLARE(14); IRQ_DECLARE(15);

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  type_attr;
    uint16_t offset_high;
} __attribute__((packed));

struct idt_pointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static void (*isr_table[32])() = {
   i686_ISR0,  i686_ISR1,  i686_ISR2,  i686_ISR3,
   i686_ISR4,  i686_ISR5,  i686_ISR6,  i686_ISR7,
   i686_ISR8,  i686_ISR9,  i686_ISR10, i686_ISR11,
   i686_ISR12, i686_ISR13, i686_ISR14, i686_ISR15,
   i686_ISR16, i686_ISR17, i686_ISR18, i686_ISR19,
   i686_ISR20, i686_ISR21, i686_ISR22, i686_ISR23,
   i686_ISR24, i686_ISR25, i686_ISR26, i686_ISR27,
   i686_ISR28, i686_ISR29, i686_ISR30, i686_ISR31
};

static void (*irq_table[16])() = {
    i686_IRQ0, i686_IRQ1, i686_IRQ2, i686_IRQ3,
    i686_IRQ4, i686_IRQ5, i686_IRQ6, i686_IRQ7,
    i686_IRQ8, i686_IRQ9, i686_IRQ10, i686_IRQ11,
    i686_IRQ12, i686_IRQ13, i686_IRQ14, i686_IRQ15
};

struct idt_entry idt[256] __attribute__((aligned(16)));
struct idt_pointer idt_p __attribute__((aligned(16)));

static inline void set_idt_entry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].offset_low  = base & 0xFFFF;
    idt[num].selector    = sel;
    idt[num].zero        = 0;
    idt[num].type_attr   = flags;
    idt[num].offset_high = (base >> 16) & 0xFFFF;
}

void idt_init(){
    idt_p.limit = sizeof(idt) - 1;
    idt_p.base = (uint32_t)&idt;

    // Clear idt
    for (int i = 0; i < 256; i++) {
        idt[i].offset_low = 0;
        idt[i].selector = 0;
        idt[i].zero = 0;
        idt[i].type_attr = 0;
        idt[i].offset_high = 0;
    }

    // Set isr
    for (int i = 0; i < 32; i++) {
        set_idt_entry(i, (uint32_t)isr_table[i], 0x08, 0x8E);
    }

    // Set irq
    for (int i = 0; i < 16; i++) {
        set_idt_entry(0x20 + i, (uint32_t)irq_table[i], 0x08, 0x8E);
    }

    asm volatile("lidt (%0)" : : "r" (&idt_p));

}
