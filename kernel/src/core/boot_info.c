#include "core/boot_info.h"

#include "core/print_vga_text.h"

struct boot_info local_boot_info;

void boot_info_init(u32 boot_info)
{
    struct boot_info* bi = (struct boot_info*)boot_info;
    local_boot_info = *bi;
}


struct boot_info boot_info_get(void)
{
    return local_boot_info;
}
