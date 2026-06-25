# Architecture

## Boot chain

```
BIOS
 └─ bootblock.asm      (MBR, 446 bytes, LBA 0)
     └─ loader.asm     (stage 2, 8 KiB, LBA 2048)
         └─ bootenv    (stage 3, C, LBA 8192)
             └─ kernel (lain_kernel.bin, ext2 partition 2 /boot/)
```

### Stage 1 - `bootblock.asm`

The BIOS loads this at `0x7C00`. It reads stage 2 from LBA 2048 into `0x1000`
via INT 13h extended read (DAP) and jumps to it. The boot drive number is stored
at `0x7E00` so stage 2 can pick it up.

### Stage 2 - `loader.asm`

Loaded at `0x1000`. Still in real mode, so this is the last chance to call
the BIOS. It collects the E820 memory map and stores it at `0x0500`, enables
the A20 line via INT 15h `0x2401`, loads bootenv from LBA 8192 into `0x2000`,
sets up a flat 32-bit GDT, switches to protected mode, and jumps to `0x2000`.

### Stage 3 - `bootenv`

32-bit ELF linked at `0x2000`, written in C with a small ASM entry point.
It initialises the IDT and remaps the PIC, reads the MBR to find the LBA of
partition 2, reads `boot/lain_kernel.bin` from the ext2 filesystem over ATA PIO,
loads the kernel at `0x100000`, builds a `boot_info` struct from the E820 data
sitting at `0x0500`, and jumps to the kernel with `EAX = 0x1BADB002` and
`EBX = &boot_info`.

### Kernel - `lain_kernel.bin`

Flat binary linked at `0x100000`. The entry point `_start` in `kernel_entry.asm`
receives the magic number and boot_info pointer from registers and calls
`kernel_main`.

---

## Physical memory layout

| Range | Content |
|---|---|
| `0x0000 – 0x04FF` | IVT and BIOS data area |
| `0x0500 – 0x05FF` | E820 map (written by loader, read by bootenv) |
| `0x0600 – 0x7BFF` | Free |
| `0x7C00 – 0x7DFF` | Stage 1 (bootblock) |
| `0x7E00` | Boot drive byte (stage 1 to stage 2 handoff) |
| `0x1000 – 0x2FFF` | Stage 2 (loader) |
| `0x2000 – 0x7FFF` | Stage 3 (bootenv) |
| `0x90000` | Stack base (loader and bootenv) |
| `0x100000+` | Kernel |

---

## Disk layout

| LBA | Size | Content |
|---|---|---|
| 0 | 1 sector | MBR (bootblock) |
| 2048 | 16 sectors | Stage 2 (loader) |
| 8192 | 40 sectors | Stage 3 (bootenv) |
| partition 2 | ext2 | `/boot/lain_kernel.bin`, future userspace |

Partition 1 (8 MiB) is unused for now. Partition 2 fills the rest of the image.

---

## Source layout

```
stand/
  bootblock.asm          stage 1
  loader.asm             stage 2
  bint/
    graphics.asm         BIOS print_string, clear_screen
    read_disk.asm        INT 13h DAP read
    e820.asm             INT 15h E820 memory map
  bootenv/
    bootenv_entry.asm    32-bit _start, calls bootenv_entry()
    bootenv.c            init sequence
    arch/
      pic.c              PIC remapping (master 0x20, slave 0x28)
      interrupts/        IDT, ISR stubs, IRQ dispatch
    core/
      kernel_loader.c    MBR parse, ext2 read, boot_info build, kernel jump
      print_vga_text.c   VGA text output
      panic.c
    drivers/
      disk.c             ATA PIO 28-bit LBA
      ext2.c             ext2 superblock, BGD, inode, dir, file read
      vga_text.c
      keyboard.c

sys/
  kernel_entry.asm       _start: push ebx/eax, call kernel_main
  kernel.c               system_init, kernel_main
  arch/                  IDT, ISR, IRQ, PIC
  core/
    boot_info.c          boot_info_init
    physical_memory_manager.c
    virtual_memory_manager.c (stub)
  utils/kshell/          kernel debug shell
```
