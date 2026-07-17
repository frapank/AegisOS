#include "drivers/disk.h"
#include "arch/io.h"
#include "utils/types.h"

constexpr u16 ATA_DATA = 0x1F0u; // 16-bit data port
constexpr u16 ATA_FEAT = 0x1F1u;
constexpr u16 ATA_SECT = 0x1F2u;  // sector count
constexpr u16 ATA_LBA0 = 0x1F3u;  // LBA  7:0
constexpr u16 ATA_LBA1 = 0x1F4u;  // LBA 15:8
constexpr u16 ATA_LBA2 = 0x1F5u;  // LBA 23:16
constexpr u16 ATA_DRIVE = 0x1F6u; // drive / LBA 27:24
constexpr u16 ATA_CMD = 0x1F7u;
constexpr u16 ATA_STATUS = 0x1F7u;

constexpr u8 ATA_CMD_READ = 0x20u;
constexpr u8 ATA_BSY = 0x80u;
constexpr u8 ATA_DRQ = 0x08u;
constexpr u8 ATA_ERR = 0x01u;
constexpr u32 ATA_TIMEOUT = 0x00FFFFFFu;

static void ata_wait_ready(void)
{
    while (inb(ATA_STATUS) & ATA_BSY)
        ;
}

static int ata_wait_drq(void)
{
    u32 i = ATA_TIMEOUT;
    u8 status;
    while (i--) {
        status = inb(ATA_STATUS);
        if (status & ATA_ERR)
            return -1;
        if (status & ATA_DRQ)
            return 0;
    }
    return -1; // timeout
}

int disk_read(u32 lba, u8 sector_count, void* buffer)
{
    u16* buf = (u16*)buffer;

    ata_wait_ready();

    outb(ATA_DRIVE, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_FEAT, 0x00);
    outb(ATA_SECT, sector_count);
    outb(ATA_LBA0, (u8)(lba));
    outb(ATA_LBA1, (u8)(lba >> 8));
    outb(ATA_LBA2, (u8)(lba >> 16));
    outb(ATA_CMD, ATA_CMD_READ);

    for (u8 s = 0; s < sector_count; s++) {
        if (ata_wait_drq() != 0)
            return -1;
        // 256 words = 512 bytes per sector
        for (int w = 0; w < 256; w++)
            buf[s * 256 + w] = inw(ATA_DATA);
    }
    return 0;
}
