ASM = nasm
GCC = i386-elf-gcc
LD  = i386-elf-ld

BUILD_DIR = build
BOOT_DIR  = boot
INC_DIR   = kernel/include
KERNEL_LOCATION = 0x1000
LINK_FILE = link.ld

GCC_FLAGS = -ffreestanding -m32 -g
INCLUDE_FLAGS = -I$(INC_DIR)

SRC_FILES = \
	kernel/src/kernel.c \
	kernel/src/core/format.c \
	kernel/src/core/print.c \
	kernel/src/drivers/keyboard.c

OBJ_FILES = $(patsubst kernel/src/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

KERNEL_ENTRY_ASM = $(BOOT_DIR)/kernel_entry.asm
KERNEL_ENTRY_OBJ = $(BUILD_DIR)/kernel_entry.o

BOOT_BIN = $(BUILD_DIR)/boot.bin

ifeq ($(v),1)
	SILENCE :=
else
	SILENCE := @
endif

all: boot

boot: $(BUILD_DIR)/everything.bin

$(BUILD_DIR)/everything.bin: $(KERNEL_ENTRY_OBJ) $(BOOT_BIN) $(OBJ_FILES)
	$(SILENCE)echo "[*] Building everything..."
	$(SILENCE)mkdir -p $(BUILD_DIR)
	$(SILENCE)$(LD) -T $(LINK_FILE) $(KERNEL_ENTRY_OBJ) $(OBJ_FILES) -o $(BUILD_DIR)/full_kernel.bin --oformat binary
	$(SILENCE)cat $(BOOT_BIN) $(BUILD_DIR)/full_kernel.bin > $(BUILD_DIR)/everything.bin

$(BUILD_DIR)/kernel_entry.o: $(KERNEL_ENTRY_ASM)
	$(SILENCE)echo "[*] Assembling kernel_entry..."
	$(SILENCE)mkdir -p $(dir $@)
	$(SILENCE)$(ASM) $< -f elf -o $@

$(BUILD_DIR)/boot.bin: $(BOOT_DIR)/boot.asm
	$(SILENCE)echo "[*] Assembling boot.bin..."
	$(SILENCE)mkdir -p $(dir $@)
	$(SILENCE)$(ASM) -f bin $< -o $@

$(BUILD_DIR)/%.o: kernel/src/%.c
	$(SILENCE)echo "[*] Compiling $< ..."
	$(SILENCE)mkdir -p $(dir $@)
	$(SILENCE)$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $< -o $@


run: all
	$(SILENCE)qemu-system-x86_64 -drive format=raw,file=$(BUILD_DIR)/everything.bin \
		-display gtk,zoom-to-fit=on

clean:
	$(SILENCE)echo "[*] Cleaning..."
	$(SILENCE)rm -rf $(BUILD_DIR)

.PHONY: all boot run clean

