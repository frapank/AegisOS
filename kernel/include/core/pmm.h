#ifndef PMM_H
#define PMM_H

#include "utils/types.h"

void pmm_init();
u32  pmm_alloc_page();
void pmm_free_page(u32 phys);

#endif
