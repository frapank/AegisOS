#include "drivers/ext2.h"
#include "drivers/disk.h"
#include "utils/types.h"

#define EXT2_MAGIC 0xEF53u
#define EXT2_ROOT_INODE 2u
#define EXT2_NDIR_BLOCKS 12u
#define EXT2_IND_BLOCK 12u

typedef struct {
    u32 s_inodes_count;
    u32 s_blocks_count;
    u32 s_r_blocks_count;
    u32 s_free_blocks_count;
    u32 s_free_inodes_count;
    u32 s_first_data_block; // 1 for 1024-byte blocks, 0 otherwise
    u32 s_log_block_size;   // block_size = 1024 << s_log_block_size
    u32 s_log_frag_size;
    u32 s_blocks_per_group;
    u32 s_frags_per_group;
    u32 s_inodes_per_group;
    u32 s_mtime;
    u32 s_wtime;
    u16 s_mnt_count;
    u16 s_max_mnt_count;
    u16 s_magic;
    u16 s_state;
    u16 s_errors;
    u16 s_minor_rev_level;
    u32 s_lastcheck;
    u32 s_checkinterval;
    u32 s_creator_os;
    u32 s_rev_level;
    u16 s_def_resuid;
    u16 s_def_resgid;
    // dynamic-revision fields (valid when s_rev_level >= 1)
    u32 s_first_ino;
    u16 s_inode_size;
} __attribute__((packed)) ext2_sb_t;

typedef struct {
    u32 bg_block_bitmap;
    u32 bg_inode_bitmap;
    u32 bg_inode_table;
    u16 bg_free_blocks_count;
    u16 bg_free_inodes_count;
    u16 bg_used_dirs_count;
    u16 bg_pad;
    u8 bg_reserved[12];
} __attribute__((packed)) ext2_bgd_t;

typedef struct {
    u16 i_mode;
    u16 i_uid;
    u32 i_size;
    u32 i_atime;
    u32 i_ctime;
    u32 i_mtime;
    u32 i_dtime;
    u16 i_gid;
    u16 i_links_count;
    u32 i_blocks;
    u32 i_flags;
    u32 i_osd1;
    u32 i_block[15]; // [0-11] direct, [12] indirect, [13-14] unused
    u32 i_generation;
    u32 i_file_acl;
    u32 i_dir_acl;
    u32 i_faddr;
    u8 i_osd2[12];
} __attribute__((packed)) ext2_inode_t;

typedef struct {
    u32 inode;
    u16 rec_len;
    u8 name_len;
    u8 file_type;
    char name[255]; // NOT null-terminated on disk
} __attribute__((packed)) ext2_dirent_t;

// Driver state
static u32 g_part_lba;
static u32 g_block_size;
static u32 g_first_data_block;
static u32 g_inodes_per_group;
static u16 g_inode_size;

// Working buffers static, go to BSS, never stack-allocated
static u8 g_block_buf[4096];  // generalpurpose block buffer
static u32 g_indir_buf[1024]; // singleindirect block pointer list

static void ext2_memcpy(void* dst, const void* src, u32 n)
{
    u8* d = (u8*)dst;
    const u8* s = (const u8*)src;
    while (n--)
        *d++ = *s++;
}

static int ext2_memcmp(const char* a, const char* b, u8 n)
{
    while (n--) {
        if (*a != *b)
            return (unsigned char)*a - (unsigned char)*b;
        a++;
        b++;
    }
    return 0;
}

static u32 block_to_lba(u32 block)
{
    return g_part_lba + block * (g_block_size / 512);
}

static int read_block(u32 block)
{
    return disk_read(
        block_to_lba(block), (u8)(g_block_size / 512), g_block_buf);
}

// Inode resolution
static int read_inode(u32 num, ext2_inode_t* out)
{
    u32 group = (num - 1) / g_inodes_per_group;
    u32 index = (num - 1) % g_inodes_per_group;

    /*
     * BGD table is in the block immediately after the superblock block.
     * Limitation: assumes the entire BGD table fits in one block.
     * True for filesystems up to ~128 MB (1 KiB blocks) or ~4 GB (4 KiB).
     */
    if (read_block(g_first_data_block + 1) != 0)
        return -1;

    ext2_bgd_t* bgd = (ext2_bgd_t*)g_block_buf + group;
    u32 inode_table = bgd->bg_inode_table;

    u32 inodes_per_block = g_block_size / g_inode_size;
    u32 inode_block = inode_table + index / inodes_per_block;
    u32 inode_offset = (index % inodes_per_block) * g_inode_size;

    if (read_block(inode_block) != 0)
        return -1;

    ext2_memcpy(out, g_block_buf + inode_offset, sizeof(ext2_inode_t));
    return 0;
}

static u32 dir_lookup(ext2_inode_t* dir, const char* name, u8 name_len)
{
    for (u32 bi = 0; bi < EXT2_NDIR_BLOCKS; bi++) {
        u32 blk = dir->i_block[bi];
        if (blk == 0)
            break;
        if (read_block(blk) != 0)
            return 0;

        u32 offset = 0;
        while (offset + 8 <= g_block_size) {
            ext2_dirent_t* de = (ext2_dirent_t*)(g_block_buf + offset);
            if (de->rec_len < 8)
                break; // malformed

            if (de->inode != 0 && de->name_len == name_len &&
                ext2_memcmp(de->name, name, name_len) == 0)
                return de->inode;

            offset += de->rec_len;
        }
    }
    return 0;
}

static u32 path_lookup(const char* path)
{
    u32 current = EXT2_ROOT_INODE;
    ext2_inode_t inode;

    while (*path) {
        while (*path == '/')
            path++;
        if (*path == '\0')
            break;

        const char* seg = path;
        while (*path && *path != '/')
            path++;
        u8 len = (u8)(path - seg);

        if (read_inode(current, &inode) != 0)
            return 0;
        current = dir_lookup(&inode, seg, len);
        if (current == 0)
            return 0;
    }
    return current;
}

static u32 read_file_data(ext2_inode_t* inode, void* buffer, u32 max_size)
{
    u8* dst = (u8*)buffer;
    u32 size = (inode->i_size < max_size) ? inode->i_size : max_size;
    u32 copied = 0;
    u32 chunk;

    // Direct blocks [0–11]
    for (u32 b = 0; b < EXT2_NDIR_BLOCKS && copied < size; b++) {
        if (inode->i_block[b] == 0)
            break;
        if (read_block(inode->i_block[b]) != 0)
            return copied;

        chunk = g_block_size;
        if (copied + chunk > size)
            chunk = size - copied;
        ext2_memcpy(dst + copied, g_block_buf, chunk);
        copied += chunk;
    }

    // Single-indirect block [12]
    if (copied < size && inode->i_block[EXT2_IND_BLOCK] != 0) {
        u32 ptrs = g_block_size / sizeof(u32);

        if (read_block(inode->i_block[EXT2_IND_BLOCK]) != 0)
            return copied;
        ext2_memcpy(g_indir_buf, g_block_buf, ptrs * sizeof(u32));

        for (u32 i = 0; i < ptrs && copied < size; i++) {
            if (g_indir_buf[i] == 0)
                break;
            if (read_block(g_indir_buf[i]) != 0)
                return copied;

            chunk = g_block_size;
            if (copied + chunk > size)
                chunk = size - copied;
            ext2_memcpy(dst + copied, g_block_buf, chunk);
            copied += chunk;
        }
    }

    return copied;
}

u32 ext2_read_file(u32 partition_lba,
                   const char* path,
                   void* buffer,
                   u32 max_size)
{
    g_part_lba = partition_lba;

    // Superblock: byte offset 1024 from partition start = LBA +2, 2 sectors
    static u8 sb_buf[1024];
    if (disk_read(partition_lba + 2, 2, sb_buf) != 0)
        return 0;

    ext2_sb_t* sb = (ext2_sb_t*)sb_buf;
    if (sb->s_magic != EXT2_MAGIC)
        return 0;

    g_block_size = 1024u << sb->s_log_block_size;
    g_first_data_block = sb->s_first_data_block;
    g_inodes_per_group = sb->s_inodes_per_group;
    g_inode_size = (sb->s_rev_level >= 1u) ? sb->s_inode_size : 128u;

    if (g_block_size > sizeof(g_block_buf))
        return 0; // unsupported size

    u32 ino = path_lookup(path);
    if (ino == 0)
        return 0;

    ext2_inode_t inode;
    if (read_inode(ino, &inode) != 0)
        return 0;

    return read_file_data(&inode, buffer, max_size);
}
