#include "drivers/vga_text.h"
#include "arch/io.h"

void vga_update_cursor(u16 x, u16 y)
{
    u16 pos = y * VGA_WIDTH + x;

    outb(0x3D4, 0x0E);
    outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);
}

void vga_put_char(u8 c, u8 forecolour, u8 backcolour, u32 x, u32 y)
{
    u16 attrib = (backcolour << 4) | (forecolour & 0x0F);
    volatile u16* video = (volatile u16*)VIDEO_MEMORY;
    video[y * VGA_WIDTH + x] = c | (attrib << 8);

    u32 cursor_x = x + 1;
    u32 cursor_y = y;
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }

    if (cursor_y < VGA_HEIGHT)
        video[cursor_y * VGA_WIDTH + cursor_x] = ' ' | (attrib << 8);
    else
        cursor_y = VGA_HEIGHT - 1;

    vga_update_cursor((u16)cursor_x, (u16)cursor_y);
}
