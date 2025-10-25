#include "arch/idt.h"

extern void i686_ISR0();
extern void i686_ISR1();
extern void i686_ISR2();
extern void i686_ISR3();
extern void i686_ISR4();
extern void i686_ISR5();
extern void i686_ISR6();
extern void i686_ISR7();
extern void i686_ISR8();
extern void i686_ISR9();
extern void i686_ISR10();
extern void i686_ISR11();
extern void i686_ISR12();
extern void i686_ISR13();
extern void i686_ISR14();
extern void i686_ISR15();
extern void i686_ISR16();
extern void i686_ISR17();
extern void i686_ISR18();
extern void i686_ISR19();
extern void i686_ISR20();
extern void i686_ISR21();
extern void i686_ISR22();
extern void i686_ISR23();
extern void i686_ISR24();
extern void i686_ISR25();
extern void i686_ISR26();
extern void i686_ISR27();
extern void i686_ISR28();
extern void i686_ISR29();
extern void i686_ISR30();
extern void i686_ISR31();

extern void i686_IRQ0();
extern void i686_IRQ1();
extern void i686_IRQ2();
extern void i686_IRQ3();
extern void i686_IRQ4();
extern void i686_IRQ5();
extern void i686_IRQ6();
extern void i686_IRQ7();
extern void i686_IRQ8();
extern void i686_IRQ9();
extern void i686_IRQ10();
extern void i686_IRQ11();
extern void i686_IRQ12();
extern void i686_IRQ13();
extern void i686_IRQ14();
extern void i686_IRQ15();

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

void set_idt_entry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
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
