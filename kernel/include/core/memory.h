#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
void* kmemset(void* ptr, uint8_t value, uint32_t count);
void* kmemset16(void* ptr, uint16_t value, uint32_t count);
void* kmemset32(void* ptr, uint32_t value, uint32_t count);

void *kmemmove(void *dest, const void *src, unsigned int n);

#endif
