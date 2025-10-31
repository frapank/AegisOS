#include "core/memory.h"

void* memset(void* ptr, uint8_t value, uint32_t count){
    uint8_t* p = (uint8_t*)ptr;
    for(uint32_t i = 0; i < count; i++){
        p[i] = value;
    }
    return ptr;
}
