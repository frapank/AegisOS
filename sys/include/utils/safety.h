#pragma once

#if defined(__clang__) || defined(__GNUC__)
#define ARGS_NONNULL(...)   __attribute__((nonnull(__VA_ARGS__)))
#else
#define ARGS_NONNULL(...)
#endif
