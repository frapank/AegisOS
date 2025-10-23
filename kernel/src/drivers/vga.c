#include "drivers/vga.h"
#include <stdint.h>

void vga_put_char(unsigned char c, unsigned char forecolour,
                unsigned char backcolour, int x, int y) {
    uint16_t attrib = (backcolour << 4) | (forecolour & 0x0F);
    volatile uint16_t *where =
        (volatile uint16_t *)VIDEO_MEMORY + (y * VGA_WIDTH + x);
    *where = c | (attrib << 8);
}

