#include "arch/interrupts/idt.h"
#include "arch/io.h"
#include "arch/pic.h"
#include "core/print.h"
#include "utils/kshell/kshell.h"

void kernel_main() 
{
    idt_init();
    pic_init();
    sti();

    kshell_start();
    
    printk_color("No programming running, starting loop...", VGA_COLOR_RED, VGA_COLOR_BLACK);
    for(;;);

    return;
}
