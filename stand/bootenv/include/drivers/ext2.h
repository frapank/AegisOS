#pragma once
#include "utils/types.h"

/* Read a file from an ext2 partition into memory
 *
 * partition_lba = first LBA of the ext2 partition (read from MBR)
 * path          = root-relative path, e.g. "boot/lain_kernel.bin
 * buffer        = destination address
 * max_size      = maximum bytes to read
 *
 * Returns bytes read on success, 0 on any error 
 */
u32 ext2_read_file(u32 partition_lba, const char *path,
                   void *buffer, u32 max_size);
