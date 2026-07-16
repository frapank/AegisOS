#pragma once

#include "utils/types.h"

[[gnu::nonnull(2)]] void uint_to_hex(u32 n, c8* buffer);
[[gnu::nonnull(2)]] void int_to_ascii(u32 n, c8* buffer);
[[nodiscard]] [[gnu::nonnull(1)]] u32 asciihex_to_uint(c8* buffer);
