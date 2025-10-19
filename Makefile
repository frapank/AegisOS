ASM = nasm
GCC = i386-elf-gcc
LD  = i386-elf-ld

BUILD_DIR = build
BOOT_DIR  = boot
INC_DIR   = kernel/include
LINK_FILE = link.ld

GCC_FLAGS = -ffreestanding -m32
INCLUDE_FLAGS = -I$(INC_DIR)

ifeq ($(v),1)
	SILENCE :=
else
	SILENCE := @
endif

all: boot kernel

boot:
	$(SILENCE)echo "[*] Assembling kernel_entry..."
	$(SILENCE)mkdir -p $(BUILD_DIR)
	$(SILENCE)$(ASM) $(BOOT_DIR)/kernel_entry.asm -f elf -o $(BUILD_DIR)/kernel_entry.o

	$(SILENCE)echo "[*] Assembling boot.bin..."
	$(SILENCE)mkdir -p $(BUILD_DIR)
	$(SILENCE)$(ASM) -f bin $(BOOT_DIR)/boot.asm -o $(BUILD_DIR)/boot.bin

kernel: boot

	$(SILENCE)echo "[*] Compiling kernel/src/*.c-asm -> $(BUILD_DIR)/ ..."
	$(SILENCE)$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c kernel/src/kernel.c -o $(BUILD_DIR)/kernel.o
	$(SILENCE)$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c kernel/src/core/format.c -o $(BUILD_DIR)/format.o
	$(SILENCE)$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c kernel/src/core/print.c -o $(BUILD_DIR)/print.o
	$(SILENCE)$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c kernel/src/drivers/keyboard.c -o $(BUILD_DIR)/keyboard.o

	$(SILENCE)$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c kernel/src/arch/idt.c -o $(BUILD_DIR)/idt.o
	$(SILENCE)$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c kernel/src/arch/isr.c -o $(BUILD_DIR)/isr.o
	$(SILENCE)$(ASM) -f elf kernel/src/arch/isr.asm -o $(BUILD_DIR)/isr_asm.o

	$(SILENCE)echo "[*] Building everything..."

	$(SILENCE)$(LD) -T $(LINK_FILE) \
	    $(BUILD_DIR)/kernel_entry.o \
	    $(BUILD_DIR)/kernel.o \
	    $(BUILD_DIR)/format.o \
	    $(BUILD_DIR)/print.o \
	    $(BUILD_DIR)/keyboard.o \
	    $(BUILD_DIR)/idt.o \
	    $(BUILD_DIR)/isr.o \
	    $(BUILD_DIR)/isr_asm.o \
	    -o $(BUILD_DIR)/full_kernel.bin \
	    --oformat binary


	$(SILENCE)cat $(BUILD_DIR)/boot.bin $(BUILD_DIR)/full_kernel.bin > $(BUILD_DIR)/everything.bin

run: all
	$(SILENCE)qemu-system-x86_64 -drive format=raw,file=$(BUILD_DIR)/everything.bin \
		-display gtk,zoom-to-fit=on

clean:
	$(SILENCE)echo "[*] Cleaning..."
	$(SILENCE)rm -rf $(BUILD_DIR)

.PHONY: all boot kernel run clean


