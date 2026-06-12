# Include vars
ROOT := $(CURDIR)
export ROOT
ifeq ("$(wildcard config.mk)","")
$(error File config.mk not found! Run ./configure to generate it)
endif

include config.mk
export ASM CC LD OBJCOPY FLAGS

# Vars
SYS_DIR			:= $(ROOT)/sys
STAND_DIR		:= $(ROOT)/stand
BUILD_DIR		:= $(ROOT)/build
STAND_BUILD_DIR := $(BUILD_DIR)/stand
SYS_BUILD_DIR 	:= $(BUILD_DIR)/sys
export SYS_DIR STAND_DIR BUILD_DIR STAND_BUILD_DIR SYS_BUILD_DIR

DISK_IMG		:= disk.img
SCRIPTS_DIR		:= scripts
MAKE_IMG_SCRIPT	:= $(SCRIPTS_DIR)/img_maker.sh
RUN_SCRIPT		:= $(SCRIPTS_DIR)/img_run.sh
FLASH_SCRIPT	:= $(SCRIPTS_DIR)/img_flash.sh

CLANG_FORMAT     := clang-format
CLANG_FMT_FLAGS  := --style=file
export CLANG_FORMAT CLANG_FMT_FLAGS

# MSG
MESS			:= printf
RESET       	:= \033[0m
RED         	:= \033[31m
GREEN       	:= \033[32m
YELLOW      	:= \033[33m
MAGENTA     	:= \033[35m
CYAN        	:= \033[36m

ifneq ($(TERM),dumb)
  ifneq (, $(shell command -v tput 2>/dev/null))
    RESET  := $(shell tput sgr0)
    RED    := $(shell tput setaf 1)
    GREEN  := $(shell tput setaf 2)
    YELLOW := $(shell tput setaf 3)
    MAGENTA:= $(shell tput setaf 5)
    CYAN   := $(shell tput setaf 6)
  endif
endif
export MESS RESET RED GREEN YELLOW MAGENTA CYAN

# Compile
all: stand sys

# Sys
sys:
	@$(MAKE) -C $(SYS_DIR)/

# Stand
stand:
	@$(MAKE) -C $(STAND_DIR)/

# Format
format: format-stand format-sys

format-sys:
	@$(MAKE) -C $(SYS_DIR)/ format

format-stand:
	@$(MAKE) -C $(STAND_DIR)/ format

# VM
img-clean:
	@$(MESS) '[$(RED)CLEAN$(RESET)] %s\n' 'Removing $(DISK_IMG)'
	@rm -f $(DISK_IMG)

img-create:
	@$(MESS) '[$(YELLOW)IMG$(RESET)] %s\n' 'Creating $(DISK_IMG)'
	@./$(MAKE_IMG_SCRIPT) $(DISK_IMG) $(VMDISK)

img-flash:
	@$(MESS) '[$(YELLOW)FLASH$(RESET)] %s\n' 'Flashing $(DISK_IMG)'
	@./$(FLASH_SCRIPT) $(DISK_IMG) $(STAND_BUILD_DIR)/bootblock.bin \
		$(STAND_BUILD_DIR)/loader.bin $(STAND_BUILD_DIR)/bootenv.bin $(SYS_BUILD_DIR)/full_kernel.bin

img-run:
	@$(MESS) '[$(YELLOW)QEMU$(RESET)] %s\n' 'Starting VM'
	@./$(RUN_SCRIPT) $(DISK_IMG) $(VMRAM)

# Utils
clean:
	@$(MESS) '[$(RED)CLEAN$(RESET)] %s\n' "Remove 'build/'"
	@rm -rf $(BUILD_DIR)

clean-stand:
	@$(MESS) '[$(RED)CLEAN$(RESET)] %s\n' "Remove 'build/stand/'"
	@rm -rf $(STAND_BUILD_DIR)

clean-sys:
	@$(MESS) '[$(RED)CLEAN$(RESET)] %s\n' "Remove 'build/sys/'"
	@rm -rf $(SYS_BUILD_DIR)

# PHONY
.PHONY: all sys stand img-clean img-create img-flash img-run \
        clean clean-stand clean-sys \
        format format-sys format-stand
