all: boot

BUILD_DIR 		:= ../build

boot:
	@mkdir -p $(BUILD_DIR)
	@$(MESS) '[$(RED)ASM$(RESET)] %s\n' 'bootblock.asm'
	@$(ASM) -f bin bootblock.asm -o $(BUILD_DIR)/bootblock.bin
	@$(MESS) '[$(RED)ASM$(RESET)] %s\n' 'loader.asm'
	@$(ASM) -f bin loader.asm -o $(BUILD_DIR)/loader.bin

.PHONY: boot
