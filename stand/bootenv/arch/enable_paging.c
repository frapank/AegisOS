#include "arch/enable_paging.h"

#include "utils/types.h"

#define ALIGN(size) __attribute__((aligned(size)))
#define PAGE_SIZE_BYTE 4096
#define TABLE_IN_DIR 1024
#define PAGE_IN_TABLE 1024

typedef u32 ptable_entry;
typedef u32 pdir_entry;
typedef u32 physical_address;
typedef u32 virtual_address;

enum PAGE_TABLE_FLAGS {
    PT_PRESENT = 0x01,
    PT_READ_WRITE = 0x02,
    PT_USER = 0x04,
    PT_WRITE_THROUGH = 0x08,
    PT_CACHE_DISABLE = 0x10,
    PT_ACCESED = 0x20,
    PT_DIRTY = 0x40,
    PT_PAT = 0x80,
    PT_GLOBAL = 0x100,
    PT_FRAME = 0x7FFFF000,
};

enum PAGE_DIR_FLAGS {
    PD_PRESENT = 0x01,
    PD_READ_WRITE = 0x02,
    PD_USER = 0x04,
    PD_WRITE_THROUGH = 0x08,
    PD_CACHE_DISABLE = 0x10,
    PD_ACCESED = 0x20,
    PD_DIRTY = 0x40,
    PD_PAGE_SIZE = 0x80,
    PD_GLOBAL = 0x100,
    PD_FRAME = 0x7FFFF000,
};

struct page_dir {
    pdir_entry entries[TABLE_IN_DIR];
};

struct page_table {
    ptable_entry entries[PAGE_IN_TABLE];
};

static struct page_dir current_page_dir;

static inline void paging_load_dir(void)
{
    __asm__("mov %0, %%cr3;"
            ""
            :
            : "r"(current_page_dir)
            : "memory");
}

static void enable_cr0(void)
{
    u32 cr0;
    __asm__("mov %%cr0, %0;"
            "or $0x80000000, %0;"
            "mov %0, %%cr0;"
            : "+r"(cr0)
            :
            : "memory");
}

void enable_paging(void)
{
}
