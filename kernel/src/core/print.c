#include "core/print.h"

#include "core/format.h"

#define DEFAULT_BACKGROUND_COLOR 0
#define DEFAULT_TEXT_COLOR 15

static struct line_data cursor = {0, 0};

static void scroll_if_needed() {
    // Todo
}

void print_color(char *msg, enum vga_color text, enum vga_color background,
                 ...) {
    __builtin_va_list args;
    __builtin_va_start(args, background);
    vprint_color(msg, text, background, args);
    __builtin_va_end(args);
}

void print(char *msg, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, msg);
    vprint_color(msg, DEFAULT_TEXT_COLOR, DEFAULT_BACKGROUND_COLOR, args);
    __builtin_va_end(args);
}

void vprint_color(char *msg, enum vga_color text, enum vga_color background,
                  __builtin_va_list args) {
    for (int i = 0; msg[i] != '\0'; i++) {
        char ch = msg[i];
        switch (ch) {
            case '\n':
                cursor.x = 0;
                cursor.y++;
                scroll_if_needed();
                break;

            case '\r':
                cursor.x = 0;
                scroll_if_needed();
                break;
            case '%':
                if (msg[i + 1] == 'd') {
                    int val = __builtin_va_arg(args, int);
                    char ascii_buffer[21];
                    int_to_ascii(val, ascii_buffer);
                    print_color(ascii_buffer, text, background);
                    i++;
                    break;
                }

                if (msg[i + 1] == 'h') {
                    unsigned int uval = __builtin_va_arg(args, unsigned int);
                    char hex_buffer[12];
                    uint_to_hex(uval, hex_buffer);
                    print_color(hex_buffer, text, background);
                    i++;
                    break;
                }

                if (msg[i + 1] == 'c') {
                    int c = __builtin_va_arg(args, int);
                    char cb[2];
                    cb[0] = (char)c;
                    cb[1] = '\0';
                    print_color(cb, text, background);
                    i++;
                    break;
                }

                if (msg[i + 1] == 's') {
                    char *s = __builtin_va_arg(args, char *);
                    for (int j = 0; s[j] != '\0'; j++) {
                        if (cursor.x >= VGA_WIDTH) {
                            cursor.x = 0;
                            cursor.y++;
                            scroll_if_needed();
                        }
                        vga_put_char((unsigned char)s[j], text, background,
                                     cursor.x, cursor.y);
                        cursor.x++;
                    }
                    i++;
                    break;
                }

            default:
                if (cursor.x >= VGA_WIDTH) {
                    cursor.x = 0;
                    cursor.y++;
                    scroll_if_needed();
                }
                vga_put_char((unsigned char)ch, text, background, cursor.x,
                             cursor.y);
                cursor.x++;
        }
    }
}

void new_line() {
    cursor.x = 0;
    cursor.y++;
    scroll_if_needed();
}

