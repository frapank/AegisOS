#pragma once

#include "drivers/vga/vga_text.h"

struct line_data {
    u32 x;
    u32 y;
};

void printk(char *msg, ...);

void clear_screenk(void);
void new_linek(void);
