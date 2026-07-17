#pragma once

#include "utils/types.h"

struct [[gnu::packed]] memory_map {
    u64 base;
    u64 length;
    u32 type;
    u32 acpi_extended;
};

struct [[gnu::packed]] boot_info {
    u32 boot_mode;
    u32 kernel_start_address;
    u32 mem_map_entries_count;
    u32 _padding;
    struct memory_map mem_map_entries[12];
};
