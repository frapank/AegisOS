#ifndef PRINT_H
#define PRINT_H

#include "drivers/vga/vga.h"

struct line_data {
    int x;
    int y;
};

void printk(char *msg, ...);
void printk_color(char *msg, enum vga_color text, enum vga_color background, ...);
void vprint_color(char *msg, enum vga_color text, enum vga_color background, __builtin_va_list args);

void kclear_screen(void);
void knew_line(void);

#endif
