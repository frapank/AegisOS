#pragma once

#include "utils/types.h"
#include "utils/safety.h"

ARGS_NONNULL(2) void uint_to_hex(u32 n, c8* buffer);
ARGS_NONNULL(2) void int_to_ascii(u32 n, c8* buffer);
[[nodiscard]] ARGS_NONNULL(1) u32 asciihex_to_uint(c8* buffer);
