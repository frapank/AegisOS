#include "arch/pic.h"
#include "arch/io.h"
#include "utils/types.h"

#define PIC_MASTER_COMMAND 0x20
#define PIC_MASTER_DATA    0x21
#define PIC_SLAVE_COMMAND  0xA0
#define PIC_SLAVE_DATA     0xA1

#define INIT_AND_WAIT_ICW4 0x11

#define OFFSET_MASTER      0x20
#define OFFSET_SLAVE       0x28

static void pic_remap(void)
{
    // Save the mask
    u8 mask1 = inb(PIC_MASTER_DATA);
    u8 mask2 = inb(PIC_SLAVE_DATA);

    // Send ICW1
    outb(PIC_MASTER_COMMAND, INIT_AND_WAIT_ICW4);
    outb(PIC_SLAVE_COMMAND, INIT_AND_WAIT_ICW4);

    // Send ICW2
    outb(PIC_MASTER_DATA, OFFSET_MASTER);
    outb(PIC_SLAVE_DATA, OFFSET_SLAVE);

    // Send ICW3
    outb(PIC_MASTER_DATA, 0x04);
    outb(PIC_SLAVE_DATA, 0x02);

    // Send ICW4 ( set 8080 mode)
    outb(PIC_MASTER_DATA, 0x01);
    outb(PIC_SLAVE_DATA, 0x01);

    // Restore mask
    outb(PIC_MASTER_DATA, mask1);
    outb(PIC_SLAVE_DATA, mask2);

}

void pic_init(void)
{
    pic_remap();
}


