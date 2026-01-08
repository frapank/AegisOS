# Lain OS
**Lain OS** is my very first 32-bit operating system project made in C. It’s part of my learning journey. I want to see how far I can take it.

## Previews of the OS
| KShell                  |
| ------------------------|
| ![preview4](screen.png) | 

The LainOS kernel runs in **32-bit mode** and is written mainly in simple C with a few Intel assembly sections. I keep the codebase clean and easy to read, avoiding excessive abstraction; as a result, code clarity and quality are often favored over micro-optimizations.  
The bootloader is custom and implemented in assembly, and I plan to expand it to be more GRUB-like. It currently supports only **Legacy BIOS**, but my goal is to add **UEFI** boot support in the future.

A growing set of technical documents explaining the internals of the OS can be found here:
* [Documentation index](docs/index.md)
* [How to build](docs/build.md)

### Roadmap
| Component              | Status    |
| ---------------------- | --------- |
| Bootloader             |  Done     |
| Kernel                 |  Done     |
| IDT                    |  Done     |
| ISR / IRQ Manager      |  Done     |
| PIC                    |  Done     |
| Keyboard & VGA Drivers |  Done     |
| Print Driver           |  Done     |
| Mini Shell             |  Done     |
| Paging                 |  Not yet  |
| Virtual Memory         |  InDev    |
| User Mode              |  Not yet  |
| File System            |  Not yet  |


