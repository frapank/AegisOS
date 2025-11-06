#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
void* memset(void* ptr, uint8_t value, uint32_t count);
void* memset16(void* ptr, uint16_t value, uint32_t count);
void* memset32(void* ptr, uint32_t value, uint32_t count);

void *memmove(void *dest, const void *src, unsigned int n);

#endif
