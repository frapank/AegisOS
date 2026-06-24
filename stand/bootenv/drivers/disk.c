#include "drivers/disk.h"
#include "arch/io.h"
#include "utils/types.h"

#define ATA_DATA 0x1F0 // 16-bit data port
#define ATA_FEAT 0x1F1
#define ATA_SECT 0x1F2  // sector count
#define ATA_LBA0 0x1F3  // LBA  7:0
#define ATA_LBA1 0x1F4  // LBA 15:8
#define ATA_LBA2 0x1F5  // LBA 23:16
#define ATA_DRIVE 0x1F6 // drive / LBA 27:24
#define ATA_CMD 0x1F7
#define ATA_STATUS 0x1F7

#define ATA_CMD_READ 0x20
#define ATA_BSY 0x80
#define ATA_DRQ 0x08
#define ATA_ERR 0x01
#define ATA_TIMEOUT 0x00FFFFFFu

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
