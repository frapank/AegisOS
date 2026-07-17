#include "arch/pic.h"
#include "arch/io.h"
#include "utils/types.h"

constexpr u16 PIC_MASTER_COMMAND = 0x20u;
constexpr u16 PIC_MASTER_DATA = 0x21u;

constexpr u16 PIC_SLAVE_COMMAND = 0xA0u;
constexpr u16 PIC_SLAVE_DATA = 0xA1u;

constexpr u8 ICW1_INIT_ICW4 = 0x11u;
constexpr u8 ICW4_8086_MODE = 0x01u;

constexpr u8 OFFSET_MASTER = 0x20u;
constexpr u8 OFFSET_SLAVE = 0x28u;

constexpr u8 PIC_SLAVE_IRQ_LINE = 0x04u;
constexpr u8 PIC_SLAVE_CASCADE_ID = 0x02u;

static void pic_remap(void)
{
    // Save masks
    const u8 master_mask = inb(PIC_MASTER_DATA);
    const u8 slave_mask = inb(PIC_SLAVE_DATA);

    // ICW1: initialize PIC and expect ICW4
    outb(PIC_MASTER_COMMAND, ICW1_INIT_ICW4);
    outb(PIC_SLAVE_COMMAND, ICW1_INIT_ICW4);

    // ICW2: interrupt vector offsets
    outb(PIC_MASTER_DATA, OFFSET_MASTER);
    outb(PIC_SLAVE_DATA, OFFSET_SLAVE);

    // ICW3: cascade configuration
    outb(PIC_MASTER_DATA, PIC_SLAVE_IRQ_LINE);
    outb(PIC_SLAVE_DATA, PIC_SLAVE_CASCADE_ID);

    // ICW4: 8086 mode
    outb(PIC_MASTER_DATA, ICW4_8086_MODE);
    outb(PIC_SLAVE_DATA, ICW4_8086_MODE);

    // Restore masks
    outb(PIC_MASTER_DATA, master_mask);
    outb(PIC_SLAVE_DATA, slave_mask);
}

void pic_init(void)
{
    pic_remap();
}
