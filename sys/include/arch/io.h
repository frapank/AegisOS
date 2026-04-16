#ifndef BUS_H
#define BUS_H

#include "utils/types.h"
#include <stdint.h>

/* in */
static inline u8 inb(u16 port) 
{
    u8 ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline u16 inw(u16 port) 
{
    u16 ret;
    __asm__ volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline u32 inl(u16 port) 
{
    u32 ret;
    __asm__ volatile("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

/* out */
static inline void outb(u16 port, u8 value) 
{
    __asm__ volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline void outw(u16 port, u16 value) 
{
    __asm__ volatile("outw %0, %1" : : "a"(value), "Nd"(port));
}

static inline void outl(u16 port, u32 value) 
{
    __asm__ volatile("outl %0, %1" : : "a"(value), "Nd"(port));
}

/* interrupts */
static inline void cli()
{
    __asm__ volatile("cli");
}

static inline void sti()
{
    __asm__ volatile("sti");
}

static inline void hlt()
{
    __asm__ volatile("hlt");
}

#endif
