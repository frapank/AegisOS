#include "arch/interrupts/irq.h"
#include "core/print_vga_text.h"
#include "drivers/keyboard.h"
#include "utils/defhelp.h"
#include "utils/types.h"

constexpr u8 IRQ_BASE_VECTOR = 0x20u;
constexpr u8 IRQ_COUNT = 16u;

static void default_irq_handler(struct regs* regs)
{
    (void)regs;
}

#define DECLARE_DEFAULT_IRQ(n)                                                 \
    [[gnu::alias("default_irq_handler")]]                                      \
    void irq##n##_man(struct regs* r)

DECLARE_DEFAULT_IRQ(0);
DECLARE_DEFAULT_IRQ(2);
DECLARE_DEFAULT_IRQ(3);
DECLARE_DEFAULT_IRQ(4);
DECLARE_DEFAULT_IRQ(5);
DECLARE_DEFAULT_IRQ(6);
DECLARE_DEFAULT_IRQ(7);
DECLARE_DEFAULT_IRQ(8);
DECLARE_DEFAULT_IRQ(9);
DECLARE_DEFAULT_IRQ(10);
DECLARE_DEFAULT_IRQ(11);
DECLARE_DEFAULT_IRQ(12);
DECLARE_DEFAULT_IRQ(13);
DECLARE_DEFAULT_IRQ(14);
DECLARE_DEFAULT_IRQ(15);

static void irq1_man(struct regs* r)
{
    (void)r;
    keyboard_interrupt_handler();
}

static void (*const irq_handlers[IRQ_COUNT])(struct regs*) = {
    irq0_man,  irq1_man,  irq2_man,  irq3_man, irq4_man,  irq5_man,
    irq6_man,  irq7_man,  irq8_man,  irq9_man, irq10_man, irq11_man,
    irq12_man, irq13_man, irq14_man, irq15_man
};

[[gnu::cdecl]] void i686_IRQ_Handler(struct regs* regs)
{
    u8 irq = (u8)regs->int_no;

    if (likely(irq >= IRQ_BASE_VECTOR))
        irq -= IRQ_BASE_VECTOR;

    if (unlikely(irq >= IRQ_COUNT)) {
        printk("%t%bUnknown irq number: %d\n",
               VGA_COLOR_LIGHT_RED,
               VGA_COLOR_BLACK,
               irq);
        return;
    }

    irq_handlers[irq](regs);
}
