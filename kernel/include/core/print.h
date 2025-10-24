#ifndef PRINT_H
#define PRINT_H

#include "drivers/vga.h"

struct line_data {
    int x;
    int y;
};

void print(char *msg, ...);
void print_color(char *msg, enum vga_color text, enum vga_color background, ...);
void vprint_color(char *msg, enum vga_color text, enum vga_color background, __builtin_va_list args);
void clear_screen();

void new_line();

#endif
