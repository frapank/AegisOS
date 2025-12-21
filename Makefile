ASM = nasm
CAT = cat
GCC = i386-elf-gcc
LD  = i386-elf-ld
OBJCOPY = i386-elf-objcopy

BUILD_DIR = build
BOOT_DIR  = boot
SRC_DIR = kernel/src
INC_DIR   = kernel/include
LINK_FILE = link.ld

GCC_FLAGS = -ffreestanding -m32 -O2 -fno-omit-frame-pointer -g -fno-stack-protector -fno-builtin -fno-common -march=i686 -mtune=i686 -pipe
ASM_DFLAGS = -g -F dwarf
INCLUDE_FLAGS = -I$(INC_DIR)

all: boot kernel

boot:
	mkdir -p $(BUILD_DIR)
	$(ASM) -f bin                 $(BOOT_DIR)/boot.asm         -o $(BUILD_DIR)/boot.bin
	$(ASM) -f bin                 $(BOOT_DIR)/boot_second.asm  -o $(BUILD_DIR)/boot_second.bin
	$(ASM) -f elf32 $(ASM_DFLAGS) $(BOOT_DIR)/kernel_entry.asm -o $(BUILD_DIR)/kernel_entry.o
	@echo "[+] Boot done"

kernel: boot
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/kernel.c                          -o $(BUILD_DIR)/kernel.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/core/format.c                     -o $(BUILD_DIR)/format.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/core/print.c                      -o $(BUILD_DIR)/print.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/core/memory.c                     -o $(BUILD_DIR)/memory.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/core/boot_info.c                  -o $(BUILD_DIR)/boot_info.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/core/panic.c                      -o $(BUILD_DIR)/panic.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/drivers/vga/vga.c                 -o $(BUILD_DIR)/vga.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/drivers/keyboard.c                -o $(BUILD_DIR)/keyboard.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/arch/pic.c                        -o $(BUILD_DIR)/pic.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/arch/interrupts/idt.c             -o $(BUILD_DIR)/idt.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/arch/interrupts/isr.c             -o $(BUILD_DIR)/isr.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/arch/interrupts/irq.c             -o $(BUILD_DIR)/irq.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/utils/kshell/kshell.c             -o $(BUILD_DIR)/kshell.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/utils/kshell/kshell_commands.c    -o $(BUILD_DIR)/kshell_commands.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/utils/string.c                    -o $(BUILD_DIR)/string.o
	$(ASM) -f elf32 $(ASM_DFLAGS)           $(SRC_DIR)/arch/interrupts/isr.asm           -o $(BUILD_DIR)/isr_asm.o

	$(LD) -T $(LINK_FILE) \
	    $(BUILD_DIR)/kernel_entry.o \
	    $(BUILD_DIR)/kernel.o \
	    $(BUILD_DIR)/format.o \
	    $(BUILD_DIR)/print.o \
		$(BUILD_DIR)/memory.o \
	    $(BUILD_DIR)/vga.o \
	    $(BUILD_DIR)/keyboard.o \
	    $(BUILD_DIR)/boot_info.o \
	    $(BUILD_DIR)/panic.o \
		$(BUILD_DIR)/idt.o \
	    $(BUILD_DIR)/isr.o \
	    $(BUILD_DIR)/irq.o \
	    $(BUILD_DIR)/pic.o \
	    $(BUILD_DIR)/isr_asm.o \
	    $(BUILD_DIR)/kshell.o \
	    $(BUILD_DIR)/kshell_commands.o \
	    $(BUILD_DIR)/string.o \
	    -o $(BUILD_DIR)/full_kernel.elf \
	    -Map=$(BUILD_DIR)/linkmap.txt

	$(OBJCOPY) -O binary build/full_kernel.elf build/full_kernel.bin
	$(CAT) build/boot.bin build/boot_second.bin build/full_kernel.bin > build/everything.bin
	@echo "[+] Kernel done"

run: all
	@echo "[*] Starting Bochs..."
	qemu-system-x86_64 -hda $(BUILD_DIR)/everything.bin -boot c

tools:
	@echo "[*] Installing in '~/usr/local/i386elfgcc'"
	./tool_make.sh

clean:
	rm -rf $(BUILD_DIR)
	@echo "[+] Clean"

.PHONY: all boot kernel run clean
