#pragma once
#include "utils/types.h"

int disk_read(u32 lba, u8 sector_count, void *buffer);
