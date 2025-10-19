#include "core/print.h"

static struct LineData cursor = {0, 0};

static void scroll_if_needed() {
    //Todo
}

static void print_char(unsigned char c, unsigned char forecolour,
                unsigned char backcolour, int x, int y) {
    uint16_t attrib = (backcolour << 4) | (forecolour & 0x0F);
    volatile uint16_t *where =
        (volatile uint16_t *)VIDEO_MEMORY + (y * VGA_WIDTH + x);
    *where = c | (attrib << 8);
}

void print(char *msg, unsigned char color, unsigned char backcolor) {
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
                print_char((unsigned char)ch, color, backcolor, cursor.x, cursor.y);
                cursor.x++;
        }
    }
}

void new_line() {
    cursor.x = 0;
    cursor.y++;
    scroll_if_needed();
}

int get_line(){
    return cursor.y;
}

int get_column(){
    return cursor.x;
}
