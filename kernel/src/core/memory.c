#include "core/memory.h"

void* memset(void* ptr, uint8_t value, uint32_t count){
    uint8_t* p = (uint8_t*)ptr;
    for(uint32_t i = 0; i < count; i++){
        p[i] = value;
    }
    return ptr;
}

void* memset16(void* ptr, uint16_t value, uint32_t count){
    uint16_t* p = (uint16_t*)ptr;
    for(uint32_t i = 0; i < count; i++){
        p[i] = value;
    }
    return ptr;
}

void* memset32(void* ptr, uint32_t value, uint32_t count){
    uint32_t* p = (uint32_t*)ptr;
    for(uint32_t i = 0; i < count; i++){
        p[i] = value;
    }
    return ptr;
}
