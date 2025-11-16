#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stddef.h>

typedef uint8_t  u8;
typedef int8_t   s8;
typedef uint16_t u16;
typedef int16_t  s16;
typedef uint32_t u32;
typedef int32_t  s32;
typedef uint64_t u64;
typedef int64_t  s64;
typedef uintptr_t uptr;
typedef intptr_t  sptr;

_Static_assert(sizeof(u8)  == 1, "u8 should be 1 byte");
_Static_assert(sizeof(u16) == 2, "u16 should be 2 byte");
_Static_assert(sizeof(u32) == 4, "u32 should be 4 byte");
_Static_assert(sizeof(size_t) >= 4, "size_t to low");
_Static_assert(sizeof(uptr) == sizeof(void*), "uptr deve avere la dimensione di un puntatore");
_Static_assert(sizeof(sptr) == sizeof(void*), "sptr deve avere la dimensione di un puntatore");

#endif
