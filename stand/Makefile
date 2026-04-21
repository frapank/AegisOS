# Vars
BOOTENV_DIR 	:= $(STAND_DIR)/bootenv
LINK			:= bootenv_link.ld
LINK_FILE		:= $(BOOTENV_DIR)/$(LINK)
ASM_DFLAGS 		:= -g -F dwarf
BOOTENV_INC		:= $(BOOTENV_DIR)/include
INCLUDE_FLAGS 	:= -I$(BOOTENV_INC)

BOOTENV_SRC		:= $(shell find $(BOOTENV_DIR) -type f -name "*.c")
BOOTENV_OBJ 	:= $(patsubst $(BOOTENV_DIR)/%.c,$(BUILD_DIR)/%.o,$(BOOTENV_SRC))

# Build
all: boot

$(BUILD_DIR)/%.o: $(BOOTENV_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(MESS) '[$(CYAN)CC$(RESET)] %s\n' "$<"
	@$(CC) $(FLAGS) $(INCLUDE_FLAGS) -c $< -o $@

$(BUILD_DIR)/bootblock.bin: $(STAND_DIR)/bootblock.asm
	@mkdir -p $(dir $@)
	@$(MESS) '[$(RED)ASM$(RESET)] %s\n' "$<"
	@$(ASM) -f bin $< -o $@

$(BUILD_DIR)/loader.bin: $(STAND_DIR)/loader.asm
	@mkdir -p $(dir $@)
	@$(MESS) '[$(RED)ASM$(RESET)] %s\n' "$<"
	@$(ASM) -f bin $< -o $@

$(BUILD_DIR)/bootenv_entry.o: $(BOOTENV_DIR)/bootenv_entry.asm
	@mkdir -p $(dir $@)
	@$(MESS) '[$(RED)ASM$(RESET)] %s\n' "$<"
	@$(ASM) -f elf32 $(ASM_DFLAGS) $< -o $@

boot: $(BOOTENV_OBJ) $(BUILD_DIR)/bootblock.bin $(BUILD_DIR)/loader.bin $(BUILD_DIR)/bootenv_entry.o
	@$(MESS) '[$(GREEN)LD$(RESET)] %s\n' 'Linking bootenv with $(LINK)'
	$(LD) --no-warn-rwx-segments -T $(LINK_FILE) \
		$(BUILD_DIR)/bootenv_entry.o \
		$(BOOTENV_OBJ) \
		-o $(BUILD_DIR)/bootenv.elf \
		-Map=$(BUILD_DIR)/bootenv_linkmap.txt
	@$(OBJCOPY) -O binary $(BUILD_DIR)/bootenv.elf $(BUILD_DIR)/bootenv.bin

.PHONY: boot
