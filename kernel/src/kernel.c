#include "arch/idt.h"
#include "core/print.h"

void kernel_main() {

    idt_init();
    print("Welcome to your personal os!\n", COLOR_WHITE, COLOR_BLACK);
    print("What can i do for you\n", COLOR_WHITE, COLOR_BLACK);

    for(;;);

    return;
}
