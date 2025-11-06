#include "arch/interrupts/irq.h"
#include "arch/io.h"
#include "core/print.h"
#include <stdint.h>

void __attribute__((cdecl)) i686_IRQ_Handler(struct regs* regs){
    uint8_t irq = (uint8_t) regs->int_no;
    if (irq >= 0x20) irq -= 0x20;

    switch(irq){
        case 0: {
            //Todo: implement
            break;
        }
        case 1: {
            uint8_t sc = inb(0x60);
            print("kbd scancode %h\n", sc);
            //Todo: impement
            break;
        }
        default: {
            print("Irq %d\n", irq);
        }
    }
}

