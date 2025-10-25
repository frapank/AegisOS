#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>
struct regs {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp_dummy, ebx, edx, ecx, eax;
    uint32_t err_code;
    uint32_t int_no;
    uint32_t eip, cs, eflags, useresp, ss;
};

#endif
