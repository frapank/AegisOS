#include "core/physical_memory_manager.h"

#include "core/memory.h"

constexpr u32 PHMM_BLOCK_SIZE = 4096u;
constexpr u32 PHMM_FIND_ERROR = 0xFFFFFFFEu;

constexpr u32 BITS_PER_BITMAP_ENTRY = 32u;
constexpr u32 BITS_PER_BYTE = 8u;

constexpr u32 BITMAP_FULL = 0xFFFFFFFFu;

typedef u32 physical_address;

/*
 * bit 0 = free
 * bit 1 = used
 */
static u32* block_bit_map = nullptr;
static u32 blocks_max = 0;
static u32 used_blocks = 0;

static inline bool is_block_occupied(u32 bit)
{
    return block_bit_map[bit / BITS_PER_BITMAP_ENTRY] &
           (1u << (bit % BITS_PER_BITMAP_ENTRY));
}

static inline void set_block(u32 bit)
{
    block_bit_map[bit / BITS_PER_BITMAP_ENTRY] |=
        (1u << (bit % BITS_PER_BITMAP_ENTRY));
}

static inline void unset_block(u32 bit)
{
    block_bit_map[bit / BITS_PER_BITMAP_ENTRY] &=
        ~(1u << (bit % BITS_PER_BITMAP_ENTRY));
}

static u32 find_first_free_blocks(u32 block_num)
{
    if (block_num == 0)
        return PHMM_FIND_ERROR;

    for (u32 i = 0; i < blocks_max / BITS_PER_BITMAP_ENTRY; i++) {

        if (block_bit_map[i] == BITMAP_FULL)
            continue;

        for (u32 j = 0; j < BITS_PER_BITMAP_ENTRY; j++) {

            u32 bit = 1u << j;

            if (block_bit_map[i] & bit)
                continue;

            u32 start_bit = i * BITS_PER_BITMAP_ENTRY + j;

            u32 free_blocks = 0;

            for (u32 count = 0; count < block_num; count++) {

                if (!is_block_occupied(start_bit + count))
                    free_blocks++;
                else
                    break;

                if (free_blocks == block_num)
                    return start_bit;
            }
        }
    }

    return PHMM_FIND_ERROR;
}

void phmm_mark_region_free(u32 init_address, u32 size)
{
    u32 block_address = init_address / PHMM_BLOCK_SIZE;

    u32 block_cover = size / PHMM_BLOCK_SIZE;

    while (block_cover > 0) {
        unset_block(block_address++);
        used_blocks--;
        block_cover--;
    }
}

void phmm_mark_region_used(u32 init_address, u32 size)
{
    u32 block_address = init_address / PHMM_BLOCK_SIZE;

    u32 block_cover = size / PHMM_BLOCK_SIZE;

    while (block_cover > 0) {
        set_block(block_address++);
        used_blocks++;
        block_cover--;
    }
}

u32* phmm_alloc_blocks(u32 num_blocks)
{
    if ((blocks_max - used_blocks) <= num_blocks)
        return nullptr;

    u32 block_address = find_first_free_blocks(num_blocks);

    if (block_address == PHMM_FIND_ERROR)
        return nullptr;

    for (u32 i = 0; i < num_blocks; i++)
        set_block(block_address + i);

    used_blocks += num_blocks;

    return (u32*)(block_address * PHMM_BLOCK_SIZE);
}

void phmm_free_blocks(u32* addr, u32 num_blocks)
{
    u32 block_address = (u32)addr / PHMM_BLOCK_SIZE;

    for (u32 i = 0; i < num_blocks; i++)
        unset_block(block_address + i);

    used_blocks -= num_blocks;
}

u32 phmm_init(u32 init_address, u32 size)
{
    block_bit_map = (u32*)init_address;

    blocks_max = size / PHMM_BLOCK_SIZE;
    used_blocks = 0;

    kmemset(block_bit_map, 0x00, blocks_max / BITS_PER_BYTE);

    return (blocks_max + 7) / BITS_PER_BYTE;
}
