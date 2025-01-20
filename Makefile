PROJECT = memmanager

all: $(PROJECT).z64
.PHONY: all

BUILD_DIR = build
SOURCE_DIR = src
include $(N64_INST)/include/n64.mk

OBJS = $(BUILD_DIR)/memmanager.o $(BUILD_DIR)/n64fs.o $(BUILD_DIR)/menu.o $(BUILD_DIR)/mpk_hw.o $(BUILD_DIR)/nman.o

memmanager.z64: N64_ROM_TITLE = "ControllerPak Test"
memmanager.z64: $(BUILD_DIR)/memmanager.dfs

$(BUILD_DIR)/memmanager.dfs: $(wildcard filesystem/*)
$(BUILD_DIR)/memmanager.elf: $(OBJS)

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
