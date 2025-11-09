#include "arch/interrupts/irq.h"
#include "core/print.h"
#include "drivers/keyboard.h"
#include <stdint.h>
#include "utils/defhelp.h"

static void default_irq_handler(struct regs* regs){
    (void)regs;
}

#define DECLARE_IRQ_WEAK(n) \
void irq##n##_man(struct regs *r) __attribute__((weak, alias("default_irq_handler")))

DECLARE_IRQ_WEAK(0);
DECLARE_IRQ_WEAK(2);
DECLARE_IRQ_WEAK(3);
DECLARE_IRQ_WEAK(4);
DECLARE_IRQ_WEAK(5);
DECLARE_IRQ_WEAK(6);
DECLARE_IRQ_WEAK(7);
DECLARE_IRQ_WEAK(8);
DECLARE_IRQ_WEAK(9);
DECLARE_IRQ_WEAK(10);
DECLARE_IRQ_WEAK(11);
DECLARE_IRQ_WEAK(12);
DECLARE_IRQ_WEAK(13);
DECLARE_IRQ_WEAK(14);
DECLARE_IRQ_WEAK(15);

static void irq1_man(struct regs *r){
    keyboard_interrupt_handler();
}

static void (* const irq_handlers[16])(struct regs*) = {
    irq0_man,  irq1_man,  irq2_man,  irq3_man,
    irq4_man,  irq5_man,  irq6_man,  irq7_man,
    irq8_man,  irq9_man,  irq10_man, irq11_man,
    irq12_man, irq13_man, irq14_man, irq15_man
};

void __attribute__((cdecl)) i686_IRQ_Handler(struct regs* regs){
    uint8_t irq = (uint8_t) regs->int_no;
    if (likely(irq >= 0x20)) irq -= 0x20;
    
    if (unlikely(irq >= 16)) {
        printk_color("Unknown irq number: %d\n", VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK, irq);
        return;
    }

    irq_handlers[irq](regs);
}

