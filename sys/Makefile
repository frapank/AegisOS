BUILD_DIR		:= ../build
KERNEL_DIR		:= ./
SRC_DIR 		:= $(KERNEL_DIR)
INC_DIR 		:= $(KERNEL_DIR)/include
LINK			:= kernel_link.ld
LINK_FILE 		:= $(KERNEL_DIR)/$(LINK)

ASM_DFLAGS 		:= -g -F dwarf
INCLUDE_FLAGS 	:= -I$(INC_DIR)

SRC_FILES 		:= $(shell find $(SRC_DIR) -type f -name "*.c")
OBJ_FILES 		:= $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# -=== Kernel ===-
all: kernel

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(MESS) '[$(CYAN)CC$(RESET)] %s\n' "$<"
	@$(CC) $(FLAGS) $(INCLUDE_FLAGS) -c $< -o $@

$(BUILD_DIR)/kernel_entry.o: $(SRC_DIR)/kernel_entry.asm
	@mkdir -p $(dir $@)
	@$(MESS) '[$(RED)ASM$(RESET)] %s\n' "$<"
	@$(ASM) -f elf32 $(ASM_DFLAGS) $< -o $@

$(BUILD_DIR)/isr_asm.o: $(SRC_DIR)/arch/interrupts/isr.asm
	@mkdir -p $(dir $@)
	@$(MESS) '[$(RED)ASM$(RESET)] %s\n' "$<"
	@$(ASM) -f elf32 $(ASM_DFLAGS) $< -o $@

kernel: $(BUILD_DIR)/kernel_entry.o $(BUILD_DIR)/isr_asm.o $(OBJ_FILES)
	@mkdir -p $(dir $@)
	@$(MESS) '[$(GREEN)LD$(RESET)] %s\n' 'Linking kernel with $(LINK)'
	@$(LD) --no-warn-rwx-segments -T $(LINK_FILE) \
		$(BUILD_DIR)/kernel_entry.o \
		$(OBJ_FILES) \
		$(BUILD_DIR)/isr_asm.o \
	    -o $(BUILD_DIR)/full_kernel.elf \
	    -Map=$(BUILD_DIR)/kernel_linkmap.txt

	@$(MESS) '[$(MAGENTA)BIN$(RESET)] %s\n' 'Generating full_kernel.bin'
	@$(OBJCOPY) -O binary $(BUILD_DIR)/full_kernel.elf $(BUILD_DIR)/full_kernel.bin

.PHONY: kernel
