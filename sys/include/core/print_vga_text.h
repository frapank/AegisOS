#pragma once

#include "drivers/vga/vga_text.h"

struct line_data {
    int x;
    int y;
};

void printk(char *msg, ...);

void clear_screenk(void);
void new_linek(void);
