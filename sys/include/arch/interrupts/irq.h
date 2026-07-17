#pragma once

#include "arch/registers.h"

[[gnu::cdecl]] void i686_IRQ_Handler(struct regs* regs);
