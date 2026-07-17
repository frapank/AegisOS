#pragma once

#include "utils/types.h"

/*
 * base: physical start address
 * length: dimension in byte
 * type: 
 * - 1: available 
 * - 2: reserved
 * - 3: acpi reclaimable
 * - 4: acpi nvs
 * - 5: bad memory
 */
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

[[nodiscard]] struct boot_info* boot_info_init(u32 boot_info);
