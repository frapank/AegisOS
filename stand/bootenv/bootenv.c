#include "arch/interrupts/idt.h"
#include "arch/pic.h"
#include "drivers/vga_text.h"
#include "core/print_vga_text.h"

void bootenv_entry(void)
{
    idt_init();
    pic_init();

    clear_screenk();
    printk("Hello world");
}
