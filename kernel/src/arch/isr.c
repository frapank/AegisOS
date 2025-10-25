#include "arch/isr.h"
#include "core/print.h"

void __attribute__((cdecl)) i686_ISR_Handler(struct regs* regs) {

    print("isr: %d\n", regs->int_no);
    for (;;);
}

