#include "arch/idt.h"
#include "core/print.h"

void kernel_main() {

    idt_init();
    print("Welcome to your personal os!\n");

    for(;;);

    return;
}
