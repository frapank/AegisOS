#include "arch/isr.h"
#include "core/format.h"
#include "core/print.h"


void __attribute__((cdecl)) i686_ISR_Handler(struct regs* regs) {
    char buffer[16];
    int_to_ascii(regs->int_no, buffer);
    print(buffer);

    for (;;);
}

