#include "arch/interrupts/idt.h"
#include "arch/pic.h"
#include "core/kernel_loader.h"
#include "core/print_vga_text.h"
#include "drivers/keyboard.h"
#include "drivers/vga_text.h"

char* wlc_message = "\n"
                    "   /\\\n"
                    "  /  \\     AegisOS\n"
                    " / /\\ \\\n"
                    " \\ \\/ /    Experimental bootloader\n"
                    "  \\  /\n"
                    "   \\/\n"
                    "\n";

void bootenv_entry(void)
{
    idt_init();
    pic_init();

    clear_screenk();
    printk(wlc_message);

    kernel_load_and_run();

    printk("[-] Kernel returned\n");
    for (;;)
        __asm__ volatile("hlt");
}
