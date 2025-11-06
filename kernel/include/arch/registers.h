#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

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
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;

    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    uint32_t int_no;
    uint32_t err_code;

    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t useresp;
    uint32_t ss;
} __attribute__((packed)) regs_t;

#endif

