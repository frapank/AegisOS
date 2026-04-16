#include "core/memory.h"
#include "utils/types.h"

void* kmemset(void* ptr, u8 value, u32 count)
{
    u8* p = (u8*)ptr;

    for(size_t i = 0; i < count; i++)
        p[i] = value;
    
    return ptr;
}

void* kmemset16(void* ptr, u16 value, u32 count)
{
    u16* p = (u16*)ptr;

    for(size_t i = 0; i < count; i++)
        p[i] = value;
    
    return ptr;
}

void* kmemset32(void* ptr, u32 value, u32 count)
{
    u32* p = (u32*)ptr;

    for(size_t i = 0; i < count; i++)
        p[i] = value;
    
    return ptr;
}

void* kmemmove(void *dest, const void *src, size_t n) 
{
    u8 *d = (u8*)dest;
    const u8 *s = (const u8 *)src;

    if (d == s || n == 0) 
        return dest;

    if (d < s) {
        for (size_t i = 0; i < n; i++)
            d[i] = s[i];
    } else {
        for (size_t i = n; i > 0; ) {
            i--;
            d[i] = s[i];
        }
    }
    return dest;
}

