all: boot

BUILD_DIR 		:= ../build
BOOTENV_DIR 	:= bootenv
LINK			:= bootenv_link.ld
LINK_FILE		:= $(BOOTENV_DIR)/$(LINK)

boot:
	@mkdir -p $(BUILD_DIR)
	@$(MESS) '[$(RED)ASM$(RESET)] %s\n' 'bootblock.asm'
	@$(ASM) -f bin bootblock.asm -o $(BUILD_DIR)/bootblock.bin
	@$(MESS) '[$(RED)ASM$(RESET)] %s\n' 'loader.asm'
	@$(ASM) -f bin loader.asm -o $(BUILD_DIR)/loader.bin

	@$(MESS) '[$(CYAN)CC$(RESET)] %s\n' "bootenv.c"
	@$(CC) $(FLAGS) $(INCLUDE_FLAGS) -c $(BOOTENV_DIR)/bootenv.c -o $(BUILD_DIR)/bootenv.o
	@$(MESS) '[$(RED)ASM$(RESET)] %s\n' 'bootenv_entry.asm'
	@$(ASM) -f elf32 $(ASM_DFLAGS) $(BOOTENV_DIR)/bootenv_entry.asm -o $(BUILD_DIR)/bootenv_entry.o
	@$(MESS) '[$(GREEN)LD$(RESET)] %s\n' 'Linking bootenv with $(LINK)'
	@$(LD) --no-warn-rwx-segments -T $(LINK_FILE) \
		$(BUILD_DIR)/bootenv.o \
		$(BUILD_DIR)/bootenv_entry.o \
	    -o $(BUILD_DIR)/bootenv.elf \
	    -Map=$(BUILD_DIR)/bootenv_linkmap.txt
	@$(OBJCOPY) -O binary $(BUILD_DIR)/bootenv.elf $(BUILD_DIR)/bootenv.bin

.PHONY: boot
