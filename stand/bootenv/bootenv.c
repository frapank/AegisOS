#include "drivers/vga_text.h"

void bootenv_entry(void)
{
    vga_put_char('H', VGA_COLOR_BLACK, VGA_COLOR_RED, 1, 1);
    vga_put_char('i', VGA_COLOR_BLACK, VGA_COLOR_RED, 2, 1);
}
