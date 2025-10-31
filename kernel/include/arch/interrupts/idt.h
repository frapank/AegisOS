#ifndef IDT_H
#define IDT_H

#include <stdint.h>
void set_idt_entry(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void idt_init();

#endif
