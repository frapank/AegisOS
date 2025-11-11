#include "core/memory.h"

void* kmemset(void* ptr, uint8_t value, uint32_t count){
    uint8_t* p = (uint8_t*)ptr;
    for(uint32_t i = 0; i < count; i++){
        p[i] = value;
    }
    return ptr;
}

void* kmemset16(void* ptr, uint16_t value, uint32_t count){
    uint16_t* p = (uint16_t*)ptr;
    for(uint32_t i = 0; i < count; i++){
        p[i] = value;
    }
    return ptr;
}

void* kmemset32(void* ptr, uint32_t value, uint32_t count){
    uint32_t* p = (uint32_t*)ptr;
    for(uint32_t i = 0; i < count; i++){
        p[i] = value;
    }
    return ptr;
}

void* kmemmove(void *dest, const void *src, unsigned int n) {
    uint8_t *d = (uint8_t *)dest;
    const uint8_t *s = (const uint8_t *)src;

    if (d == s || n == 0) return dest;

    if (d < s) {
        for (unsigned int i = 0; i < n; i++) {
            d[i] = s[i];
        }
    } else {
        for (unsigned int i = n; i > 0; ) {
            i--;
            d[i] = s[i];
        }
    }
    return dest;
}

