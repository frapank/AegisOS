#include "arch/interrupts/idt.h"
#include "arch/io.h"
#include "arch/pic.h"
#include "core/print.h"
#include "utils/kshell/kshell.h"

void kernel_main() {
    idt_init();
    pic_init();
    sti();
    
    clear_screen();
    printk("Welcome to your personal os!\n");

    // Kernel debug shell
    kshell_start();
    

    return;
}
