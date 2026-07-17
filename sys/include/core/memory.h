#pragma once

#include "utils/types.h"

[[gnu::nonnull(1)]] void* kmemset(void* ptr, u8 value, u32 count);
[[gnu::nonnull(1)]] void* kmemset16(void* ptr, u16 value, u32 count);
[[gnu::nonnull(1)]] void* kmemset32(void* ptr, u32 value, u32 count);
[[gnu::nonnull(1)]] void* kmemmove(void *dest, const void *src, size_t n);
