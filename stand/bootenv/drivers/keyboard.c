#include "drivers/keyboard.h"

/*
* keyboard.c - Low-level PS/2 keyboard driver
*
* This driver handles keyboard input at the hardware level,
* providing a simple ring buffer and character-level API.
*
* Architecture:
*  - Keyboard generates scancodes via PS/2 controller (port 0x60)
*  - Interrupt handler reads scancodes on key press/release
*  - Modifier keys (Shift, Ctrl, Alt, CapsLock) are tracked separately
*  - Scancodes are converted to ASCII using lookup tables, with
*    support for shift-modified characters
*
* Concurrency / safety:
*  - Buffer push/pop operation disable interrupts briefly
*    to prevent race conditions betwen handler and readers
*  - getch() block using hlt until new input is available
*/

#include "arch/io.h"

/* Modificators */
#define SC_LSHIFT 0x2A
#define SC_RSHIFT 0x36
#define SC_CTRL   0x1D
#define SC_ALT    0x38
#define SC_CAPS   0x3A

/* State of modifier keys */
static volatile int shift_down = 0;
static volatile int ctrl_down  = 0;

/* Ring */
#define KEYBOARD_BUFFER_LIMIT 255

static volatile u16 kbd_keys[KEYBOARD_BUFFER_LIMIT];
static volatile u16 kbd_head = 0; // Next slot to write
static volatile u16 kbd_tail = 0; // Next slot to read

/* Conversion */
static const char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6',    /* 0x00 - 0x07 */
    '7','8','9','0','-','=','\b','\t',  /* 0x08 - 0x0F */
    'q','w','e','r','t','y','u','i',    /* 0x10 - 0x17 */
    'o','p','[',']','\n', 0,  'a','s',  /* 0x18 - 0x1F */
    'd','f','g','h','j','k','l',';',    /* 0x20 - 0x27 */
    '\'', '`', 0,  '\\','z','x','c','v',/* 0x28 - 0x2F */
    'b','n','m',',','.','/', 0,   '*',  /* 0x30 - 0x37 */
    0,  ' ', 0,   0,   0,   0,   0,   0,/* 0x38 - 0x3F */
};

static const char scancode_to_ascii_shift[128] = {
    0,  27, '!', '@','#','$','%','^',
    '&','*','(',')','_','+','\b','\t',
    'Q','W','E','R','T','Y','U','I',
    'O','P','{','}','\n', 0,  'A','S',
    'D','F','G','H','J','K','L',':',
    '"', '~', 0,  '|','Z','X','C','V',
    'B','N','M','<','>','?', 0,   '*',
    0,  ' ', 0,   0,   0,   0,   0,   0,
};

/* Ring Implementation */
static inline int kbd_is_empty(void) 
{
    return kbd_head == kbd_tail;
}

static void kbd_buffer_push(u16 char_code)
{
    u16 next = (kbd_head + 1) % KEYBOARD_BUFFER_LIMIT;
    if (next == kbd_tail) return; // Ignore
    
    kbd_keys[kbd_head] = char_code;
    kbd_head = next;
}

static u16 kbd_buffer_pop(void)
{
    u16 ret_val = -1;

    cli();
    if(!kbd_is_empty()){
        ret_val = (u16)kbd_keys[kbd_tail];
        kbd_tail = (kbd_tail + 1) % KEYBOARD_BUFFER_LIMIT;
    }
    sti();

    return ret_val; // return -1 if empty
}

char getch(void)
{
    char c;

    for (;;){
        c = kbd_buffer_pop();
        if(c != -1) return c;
        sti();
        hlt();
    }
}

void keyboard_interrupt_handler(void)
{
    u8 sc = inb(0x60);
    u8 key = sc & 0x7F;
    u8 released = sc & 0x80;

    if (key == SC_LSHIFT || key == SC_RSHIFT) {
        if (released) shift_down = 0;
        else shift_down = 1;
        return;
    }

    if(released)
        return;
    
    char ch = 0;
    if (shift_down) 
        ch = (key < 128) ? scancode_to_ascii_shift[key] : 0;
    else 
        ch = (key < 128) ? scancode_to_ascii[key] : 0;

    if (ch) 
        kbd_buffer_push(ch);
}
