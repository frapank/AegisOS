#include "arch/irq.h"
#include "core/print.h"
#include <stdint.h>

void __attribute__((cdecl)) i686_IRQ_Handler(struct regs* regs){
    uint8_t irq = (uint8_t) regs->int_no;
    if (irq >= 0x20) irq -= 0x20;
    print("Irq %d\n", irq);
}
