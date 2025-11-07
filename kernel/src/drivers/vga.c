#include "drivers/vga.h"
#include "arch/io.h"
#include <stdint.h>

void vga_update_cursor(uint16_t x, uint16_t y) {
    uint16_t pos = y * VGA_WIDTH + x;
    outb(0x3D4, 0x0E);     
    outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);
}

void vga_put_char(uint8_t c, uint8_t forecolour, uint8_t backcolour, uint32_t x, uint32_t y){
    uint16_t attrib = (backcolour << 4) | (forecolour & 0x0F);
    volatile uint16_t *where = (volatile uint16_t *)VIDEO_MEMORY + (y * VGA_WIDTH + x);
    *where = c | (attrib << 8);
    vga_update_cursor(x, y);
}


