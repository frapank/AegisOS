#include "core/panic.h"

#include "arch/io.h"
#include "core/print.h"

void panick(char* msg)
{
    cli();
    clear_screenk();
    printk_color("KERNEL PANIC:\n%s", VGA_COLOR_RED, VGA_COLOR_BLACK, msg);
    for(;;) {
        hlt();
    }
}
