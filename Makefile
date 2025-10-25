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

GCC_FLAGS = -ffreestanding -m32
INCLUDE_FLAGS = -I$(INC_DIR)

all: boot kernel

boot:
	mkdir -p $(BUILD_DIR)
	$(ASM) -f elf32 $(BOOT_DIR)/kernel_entry.asm -o $(BUILD_DIR)/kernel_entry.o
	$(ASM) -f bin $(BOOT_DIR)/boot.asm           -o $(BUILD_DIR)/boot.bin
	@echo "[+] Boot done"

kernel: boot

	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/kernel.c      -o $(BUILD_DIR)/kernel.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/core/format.c -o $(BUILD_DIR)/format.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/core/print.c  -o $(BUILD_DIR)/print.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $(SRC_DIR)/drivers/vga.c -o $(BUILD_DIR)/vga.o

	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c kernel/src/arch/idt.c -o $(BUILD_DIR)/idt.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c kernel/src/arch/isr.c -o $(BUILD_DIR)/isr.o
	$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c kernel/src/arch/irq.c -o $(BUILD_DIR)/irq.o
	$(ASM) -f elf32 kernel/src/arch/isr_asm.asm -o $(BUILD_DIR)/isr_asm.o

	$(LD) -T $(LINK_FILE) \
	    $(BUILD_DIR)/kernel_entry.o \
	    $(BUILD_DIR)/kernel.o \
	    $(BUILD_DIR)/format.o \
	    $(BUILD_DIR)/print.o \
	    $(BUILD_DIR)/vga.o \
		$(BUILD_DIR)/idt.o \
	    $(BUILD_DIR)/isr.o \
	    $(BUILD_DIR)/irq.o \
	    $(BUILD_DIR)/isr_asm.o \
	    -o $(BUILD_DIR)/full_kernel.elf \
	    -Map=$(BUILD_DIR)/linkmap.txt

	$(OBJCOPY) -O binary build/full_kernel.elf build/full_kernel.bin
	$(CAT) build/boot.bin build/full_kernel.bin > build/everything.bin
	@echo "[+] Kernel done"

run: all
	qemu-system-x86_64 -drive format=raw,file=$(BUILD_DIR)/everything.bin

clean:
	rm -rf $(BUILD_DIR)
	@echo "[+] Clean"

.PHONY: all boot kernel run clean
