#pragma once

#include "arch/registers.h"

void __attribute__((cdecl)) i686_IRQ_Handler(struct regs* regs);
