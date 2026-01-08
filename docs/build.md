## build.md

A simple guide to build LainOS.

Prerequisites
- Unix-like OS (Linux/macOS)
- sudo (only for toolchain install)
- Required tools in PATH: nasm, cat, i386-elf-gcc, i386-elf-ld, i386-elf-objcopy
- qemu-system-x86_64 to run the image

1. Install the cross toolchain (if needed)
- Make tool_make.sh executable and run it. This installs an i386-elf toolchain to /usr/local/i386elfgcc.
```bash
# follow the script output to add /usr/local/i386elfgcc/bin to your PATH
./tool_make.sh
```

2. Generate config.mk
- Make the configure script executable and run it. It detects tools and creates config.mk with compiler flags.
```bash
./configure
```
- Enable debug flags:
```bash
./configure --enable-debug
```
- Use custom tool paths:
```bash
./configure --set-gcc=/path/to/i386-elf-gcc --set-ld=/path/to/i386-elf-ld --set-objcopy=/path/to/i386-elf-objcopy --set-asm=/path/to/nasm
```

3. Build the project
```bash
make
```
- Output files:
  - build/full_kernel.elf
  - build/full_kernel.bin
  - build/everything.bin

4. Run in QEMU
```bash
make run
```
or
```bash
qemu-system-x86_64 -drive file=build/everything.bin,format=raw -m 512M -enable-kvm -boot c -serial stdio
```

5. Clean build artifacts
```bash
make clean
```
