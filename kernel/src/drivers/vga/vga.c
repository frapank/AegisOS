#include "drivers/vga/vga.h"
#include "arch/io.h"

void vga_update_cursor(u16 x, u16 y) {
    u16 pos = y * VGA_WIDTH + x;
    outb(0x3D4, 0x0E);     
    outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);
}

void vga_put_char(u8 c, u8 forecolour, u8 backcolour, u32 x, u32 y){
    u16 attrib = (backcolour << 4) | (forecolour & 0x0F);
    volatile u16 *where = (volatile u16 *)VIDEO_MEMORY + (y * VGA_WIDTH + x);
    *where = c | (attrib << 8);
    vga_update_cursor(x, y);
}


