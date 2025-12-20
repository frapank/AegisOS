#include "arch/interrupts/idt.h"
#include "arch/io.h"
#include "arch/pic.h"
#include "core/print.h"
#include "core/boot_info.h"
#include "utils/kshell/kshell.h"

#define BOOT_IDENTIFIER 0x1BADB002

void kernel_main(u32 magic, u32 boot_info)
{

    boot_info_init(boot_info);

    if(magic != BOOT_IDENTIFIER) {
        clear_screenk();
        printk_color("Wrong bootloader identifier, starting loop...\n", VGA_COLOR_RED, VGA_COLOR_BLACK);
        for(;;);
    }

    idt_init();
    pic_init();
    sti();


    kshell_start();

    printk_color("No programming running, starting loop...", VGA_COLOR_RED, VGA_COLOR_BLACK);
    for(;;);

    return;
}
