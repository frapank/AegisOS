#ifndef BOOT_INFO_H
#define BOOT_INFO_H

#include "utils/types.h"

struct boot_info {
    u8 boot_mode;
    u8 boot_flags;
};

void boot_info_init(u32 boot_info);
struct boot_info boot_info_get(void);

#endif
