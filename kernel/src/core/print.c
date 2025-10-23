#include "core/print.h"

#define DEFAULT_BACKGROUND_COLOR 0
#define DEFAULT_TEXT_COLOR 15

static struct line_data cursor = {0, 0};

static void scroll_if_needed() {
    //Todo
}

void print(char *msg){
    print_color(msg, DEFAULT_TEXT_COLOR, DEFAULT_BACKGROUND_COLOR);
}

void print_color(char *msg, enum vga_color text, enum vga_color background){
    for (int i = 0; msg[i] != '\0'; i++) {

        char ch = msg[i];
        switch (ch){
            case '\n':
                cursor.x = 0;
                cursor.y++;
                scroll_if_needed();
                break;

            case '\r': 
                cursor.x = 0;
                scroll_if_needed();
                break;
            
            default: 
                if (cursor.x >= VGA_WIDTH) {
                    cursor.x = 0;
                    cursor.y++;
                    scroll_if_needed();
                }
                vga_put_char((unsigned char)ch, text, background, cursor.x, cursor.y);
                cursor.x++;
        }
    }
}

void new_line() {
    cursor.x = 0;
    cursor.y++;
    scroll_if_needed();
}

