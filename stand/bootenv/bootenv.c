#include "arch/interrupts/idt.h"
#include "arch/pic.h"
#include "core/print_vga_text.h"
#include "drivers/vga_text.h"
#include "drivers/keyboard.h"

char* wlc_message =
"+----------------------------+\n"
"|                            |\n"
"|       L A I N   O S        |\n"
"|                            |\n"
"|   experimental bootloader  |\n"
"|                            |\n"
"+----------------------------+";

void bootenv_entry(void)
{
    // Init
    idt_init();
    pic_init();

    // User
    clear_screenk();
    printk(wlc_message);

    // Finish
}
