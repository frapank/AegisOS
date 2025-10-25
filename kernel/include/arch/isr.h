#ifndef ISR_H
#define ISR_H

#include "arch/registers.h"
void __attribute__((cdecl)) i686_ISR_Handler(struct regs* regs);

#endif
