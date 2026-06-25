# AegisOS

> [!WARNING]
> This README describes planned and conceptual features.

AegisOS is a microkernel operating system built from scratch for x86 (i686).

At its core, the kernel does as little as possible. It exists only to manage memory and isolate processes, while everything else: filesystems, networking, and system services, runs independently in user space.

The goal of AegisOS is to explore a more composable and secure operating system design, where every subsystem is replaceable, testable, and isolated by default.

See [docs/](docs/) for architecture and design details.

## Build

First, build the cross-compiler. This only needs to be done once:

```bash
./make_tools
```

This builds `i386-elf-gcc` and `i386-elf-ld` into `tools/` and takes around 10 minutes.
You need `gcc`, `g++`, `make`, `wget`, `libgmp-dev`, `libmpfr-dev`, `libmpc-dev` on the host.

Then configure the build:

```bash
./configure [options]
```

| Option | Default | Description |
|---|---|---|
| `--set-vmram=M\|G` | `512M` | QEMU RAM |
| `--set-vmdisk=N` | `1024` | Disk image size in MB |
| `--set-gcc=PATH` | `tools/bin/i386-elf-gcc` | Custom GCC path |
| `--set-ld=PATH` | `tools/bin/i386-elf-ld` | Custom LD path |
| `--set-objcopy=PATH` | `tools/bin/i386-elf-objcopy` | Custom objcopy path |
| `--set-asm=PATH` | `nasm` | Custom assembler path |
| `--enable-debug` | off | Enable debug flags and symbols |
| `--disable-kshell` | on | Disable the kernel shell |

Then:

```bash
make                # build bootloader and kernel
make img-create     # create a blank disk image
make img-flash      # write the binaries to the disk image
make img-run        # run in QEMU
```

Other useful targets:

```bash
make clean          # remove build/
make clean-stand    # remove build/stand/
make clean-sys      # remove build/sys/
make format         # clang-format all C sources
```

## Roadmap

**Done:**
- ISR/IRQ, PIC, VGA, Keyboard, Kernel Shell, PMM, Kernel Loader

**In Progress:**
- 64-bit kernel

**Planned:**
- Virtual Memory, Timer, Scheduler, Context Switch, Syscalls, User Mode, ELF Loader, Init System, User Shell, Logging, Full FS

## License

This project is licensed under the GNU GPL v3.0 - see the LICENSE file for details.
