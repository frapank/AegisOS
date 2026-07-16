#pragma once

#include "utils/types.h"
constexpr u32 VIDEO_MEMORY = 0xb8000;
constexpr u32 VGA_WIDTH = 80;
constexpr u32 VGA_HEIGHT = 25;
constexpr u8 DEFAULT_ATT = 0;

enum vga_color : u8 {
    VGA_COLOR_BLACK         = 0,
    VGA_COLOR_BLUE          = 1,
    VGA_COLOR_GREEN         = 2,
    VGA_COLOR_CYAN          = 3,
    VGA_COLOR_RED           = 4,
    VGA_COLOR_MAGENTA       = 5,
    VGA_COLOR_BROWN         = 6,
    VGA_COLOR_LIGHT_GREY    = 7,
    VGA_COLOR_DARK_GREY     = 8,
    VGA_COLOR_LIGHT_BLUE    = 9,
    VGA_COLOR_LIGHT_GREEN   = 10,
    VGA_COLOR_LIGHT_CYAN    = 11,
    VGA_COLOR_LIGHT_RED     = 12,
    VGA_COLOR_PINK          = 13,
    VGA_COLOR_YELLOW        = 14,
    VGA_COLOR_WHITE         = 15,
};

void vga_put_char(u8 c, u8 forecolour, u8 backcolour, u32 x, u32 y);
void vga_update_cursor(u16 x, u16 y);
