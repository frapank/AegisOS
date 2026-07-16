#pragma once

#include "arch/registers.h"
[[gnu::cdecl]] void i686_ISR_Handler(struct regs* regs);
