#include "core/kernel_loader.h"
#include "core/boot_info.h"
#include "core/print_vga_text.h"
#include "drivers/disk.h"
#include "drivers/ext2.h"
#include "utils/types.h"

#define KERNEL_PATH "boot/lain_kernel.bin"
#define KERNEL_LOAD_ADDR 0x100000u
#define KERNEL_MAX_SIZE (4u * 1024u * 1024u)

// Layout defined in loader.asm
#define E820_DATA_ADDR 0x0500u
#define E820_MAX_ENTRIES 12

typedef struct {
    u32 count;
    struct memory_map entries[E820_MAX_ENTRIES];
} __attribute__((packed)) e820_data_t;

// boot_info for the kernel
static struct boot_info g_boot_info;

typedef struct {
    u8 status;
    u8 chs_first[3];
    u8 type;
    u8 chs_last[3];
    u32 lba_start;
    u32 lba_size;
} __attribute__((packed)) mbr_part_t;

static u32 get_part2_lba(void)
{
    static u8 mbr[512];
    if (disk_read(0, 1, mbr) != 0)
        return 0;
    if (mbr[510] != 0x55 || mbr[511] != 0xAA)
        return 0;

    mbr_part_t* p2 =
        (mbr_part_t*)(mbr + 0x1CE); // partition 2 = 0x1BE + 1*16 = 0x1CE
    return p2->lba_start;
}

static void a20_enable(void)
{
    u8 val;
    __asm__ volatile("inb  $0x92, %0\n\t"
                     "orb  $0x02, %0\n\t"
                     "andb $0xFE, %0\n\t"
                     "outb %0,    $0x92\n\t"
                     : "=a"(val));
}

static void build_boot_info(void)
{
    e820_data_t* e820 = (e820_data_t*)E820_DATA_ADDR;

    u32 count = e820->count;
    if (count > E820_MAX_ENTRIES)
        count = E820_MAX_ENTRIES;

    g_boot_info.boot_mode = 1;
    g_boot_info.kernel_start_address = KERNEL_LOAD_ADDR;
    g_boot_info.mem_map_entries_count = count;
    g_boot_info._padding = 0;

    for (u32 i = 0; i < count; i++)
        g_boot_info.mem_map_entries[i] = e820->entries[i];
}

void kernel_load_and_run(void)
{
    a20_enable();

    printk("[*] Reading partition table...\n");
    u32 part2_lba = get_part2_lba();
    if (part2_lba == 0) {
        printk("[-] Cannot read partition table\n");
        return;
    }

    printk("[*] Loading " KERNEL_PATH "...\n");
    u32 size = ext2_read_file(
        part2_lba, KERNEL_PATH, (void*)KERNEL_LOAD_ADDR, KERNEL_MAX_SIZE);
    if (size == 0) {
        printk("[-] Kernel not found or read error\n");
        return;
    }

    build_boot_info();

    printk("[+] Jumping to kernel...\n");

    u32 magic = 0x1BADB002u;
    u32 boot_info = (u32)&g_boot_info;
    u32 entry = KERNEL_LOAD_ADDR;

    // push ebx (boot_info), push eax (magic), call kernel_main
    __asm__ volatile("mov %0, %%eax\n\t"
                     "mov %1, %%ebx\n\t"
                     "jmp *%2\n\t"
                     :
                     : "r"(magic), "r"(boot_info), "r"(entry)
                     : "eax", "ebx");
    __builtin_unreachable();
}
