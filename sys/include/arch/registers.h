#ifndef REGISTERS_H
#define REGISTERS_H

#include "utils/types.h"

// Order:
//   pusha        < eax ecx ... edi
//   mov eax, ds
//   push eax
//   mov eax, es
//   push eax
//   mov eax, fs
//   push eax
//   mov eax, gs
//   push eax
//
//   mov eax, esp
//   push eax

typedef struct regs {
    u32 gs;
    u32 fs;
    u32 es;
    u32 ds;

    u32 edi;
    u32 esi;
    u32 ebp;
    u32 esp;
    u32 ebx;
    u32 edx;
    u32 ecx;
    u32 eax;

    u32 int_no;
    u32 err_code;

    u32 eip;
    u32 cs;
    u32 eflags;
    u32 useresp;
    u32 ss;
} __attribute__((packed)) regs_t;

#endif

