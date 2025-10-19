ASM = nasm
GCC = i386-elf-gcc
LD  = i386-elf-ld

BUILD_DIR = build
BOOT_DIR  = boot
SRC_DIR   = kernel/src
INC_DIR   = kernel/include
KERNEL_LOCATION = 0x1000

GCC_FLAGS = -ffreestanding -m32 -g
INCLUDE_FLAGS = -I$(INC_DIR)

SRC_FILES := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

KERNEL_ENTRY_ASM := $(BOOT_DIR)/kernel_entry.asm
KERNEL_ENTRY_OBJ := $(BUILD_DIR)/kernel_entry.o
BOOT_BIN := $(BUILD_DIR)/boot.bin

all: boot

boot: $(BUILD_DIR)/everything.bin

$(BUILD_DIR)/everything.bin: $(KERNEL_ENTRY_OBJ) $(BOOT_BIN) $(OBJ_FILES)
	@echo "[*] Building everything..."
	@mkdir -p $(BUILD_DIR)
	@$(LD) -Ttext $(KERNEL_LOCATION) $(KERNEL_ENTRY_OBJ) $(OBJ_FILES) -o $(BUILD_DIR)/full_kernel.bin --oformat binary
	@cat $(BOOT_BIN) $(BUILD_DIR)/full_kernel.bin > $(BUILD_DIR)/everything.bin

$(BUILD_DIR)/kernel_entry.o: $(KERNEL_ENTRY_ASM)
	@echo "[*] Assembling kernel_entry..."
	@mkdir -p $(dir $@)
	@$(ASM) $< -f elf -o $@ > /dev/null

$(BUILD_DIR)/boot.bin: $(BOOT_DIR)/boot.asm
	@echo "[*] Assembling boot.bin..."
	@mkdir -p $(dir $@)
	@$(ASM) -f bin $< -o $@ > /dev/null

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "[*] Compiling $< ..."
	@mkdir -p $(dir $@)
	@$(GCC) $(GCC_FLAGS) $(INCLUDE_FLAGS) -c $< -o $@ > /dev/null

run: all
	@qemu-system-x86_64 -drive format=raw,file=$(BUILD_DIR)/everything.bin \
		-display gtk,zoom-to-fit=on

clean:
	@echo "[*] Cleaning..."
	@rm -rf $(BUILD_DIR)

.PHONY: all boot run clean

