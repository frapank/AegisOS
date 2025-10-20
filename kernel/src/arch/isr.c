#include "arch/isr.h"
//#include "core/format.h"
#include "core/print.h"

void __attribute__((cdecl)) i686_ISR_Handler(struct regs* regs) {
    //char buffer[8];
    //int_to_ascii(regs->int_no, buffer);
    //print(buffer, COLOR_CYAN, COLOR_WHITE);
    char buffer[] = "Test";
    print(buffer, COLOR_WHITE, COLOR_BLACK);
}

