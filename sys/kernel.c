#include "arch/interrupts/idt.h"
#include "arch/io.h"
#include "arch/pic.h"
#include "core/boot_info.h"
#include "core/kernel_context.h"
#include "core/panic.h"
#include "core/physical_memory_manager.h"
#include "core/print_vga_text.h"
#include "core/virtual_memory_manager.h"
#ifdef KSHELL
#include "utils/kshell/kshell.h"
#endif

constexpr u32 BOOT_IDENTIFIER = 0x1BADB002u;
extern char kernel_end;
static inline u32 kernel_end_address(void)
{
    return (u32)&kernel_end;
}

static u32 calculate_total_memory(struct boot_info* boot_info)
{
    u32 total_mem_size = 0;

    const u32 count = boot_info->mem_map_entries_count;
    for (u32 i = 0; i < count; i++) {
        total_mem_size += boot_info->mem_map_entries[i].length;
    }
    return total_mem_size;
}

static void system_init(struct kernel_context* ctx, u32 magic)
{
    if (magic != BOOT_IDENTIFIER)
        panick("Wrong bootloader identifier");

    idt_init();
    pic_init();

    constexpr u32 KB = 1024u;
    const u32 kernel_end_addr = (u32)kernel_end_address();
    u32 alloc_bytes = ctx->system_ram_kb * KB;
    u32 bitmap_size = phmm_init(kernel_end_addr, alloc_bytes);
    if (bitmap_size == 0 || bitmap_size > alloc_bytes)
        panick("phmm_init failed");

    constexpr u32 START_REGION = 0x00000000;
    u32 bytes_to_reserve = ((u32)kernel_end_addr - START_REGION) + bitmap_size;
    phmm_mark_region_used(START_REGION, bytes_to_reserve);

    // vmm_init(ctx);

    sti();
}

void kernel_main(u32 magic, u32 boot_info_addr)
{
    struct kernel_context ctx;
    ctx.kernel_boot_info = *boot_info_init(boot_info_addr);

    u32 total_mem_size = calculate_total_memory(&ctx.kernel_boot_info);
    ctx.system_ram_kb = total_mem_size / 1024;

    system_init(&ctx, magic);

#ifdef KSHELL
    kshell_start(&ctx);
#endif

    panick("No programs running");
}
