PROJECT = memmanager

all: $(PROJECT).z64
.PHONY: all

BUILD_DIR = build
SOURCE_DIR = src
include $(N64_INST)/include/n64.mk

OBJS = $(BUILD_DIR)/$(PROJECT).o $(BUILD_DIR)/n64fs.o

cpaktest.z64: N64_ROM_TITLE = "ControllerPak Test"
cpaktest.z64: $(BUILD_DIR)/$(PROJECT).dfs

$(BUILD_DIR)/$(PROJECT).dfs: $(wildcard filesystem/*)
$(BUILD_DIR)/$(PROJECT).elf: $(OBJS)

clean:
	rm -rf $(BUILD_DIR) *.z64
.PHONY: clean

-include $(wildcard $(BUILD_DIR)/*.d)
