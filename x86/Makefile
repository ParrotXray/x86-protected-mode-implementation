OS_ARCH := x86

BUILD_DIR := build
OBJECT_DIR := $(BUILD_DIR)/obj
BIN_DIR := $(BUILD_DIR)/bin

OS_NAME = system
OS_BIN = $(OS_NAME).bin
OS_ELF = $(OS_NAME).elf
OS_IMG = $(OS_NAME).img

QEMU_MON_PORT := 45454
CC := i686-elf-

O := -O3
W := -Wall -Wextra
CFLAGS := -std=gnu99 -ffreestanding -ffunction-sections -fdata-sections $(O) $(W)
LDFLAGS := -ffreestanding $(O) -fno-pie -fno-stack-protector -nostdlib -nostdinc -lgcc

SOURCE_FILES := $(wildcard source/*.c source/*.S) 
OBJ_FILES := $(patsubst source/%.c, $(OBJECT_DIR)/%.o, $(patsubst source/%.S, $(OBJECT_DIR)/%.o, $(SOURCE_FILES)))

$(OBJECT_DIR) $(BIN_DIR) $(IMG_DIR):
	@mkdir -p $@

$(OBJECT_DIR)/%.o: source/%.S | $(OBJECT_DIR)
	$(CC)gcc $(CFLAGS) -c $< -o $@

$(OBJECT_DIR)/%.o: source/%.c | $(OBJECT_DIR)
	$(CC)gcc $(CFLAGS) -c $< -o $@

$(BIN_DIR)/$(OS_BIN): $(OBJ_FILES) | $(BIN_DIR)
	$(CC)ld -Ttext=0x7c00 -m elf_i386 $(OBJ_FILES) -o $(BIN_DIR)/$(OS_ELF) 
	${TOOL_PREFIX}objcopy -O binary $(BIN_DIR)/$(OS_ELF) $(BIN_DIR)/$(OS_BIN)
	${TOOL_PREFIX}objdump -x -d -S $(BIN_DIR)/$(OS_ELF) > $(BIN_DIR)/$(OS_NAME)_dis.txt	
	${TOOL_PREFIX}readelf -a $(BIN_DIR)/$(OS_ELF) > $(BIN_DIR)/$(OS_NAME)_elf.txt
	dd if=/dev/zero of=$(BIN_DIR)/$(OS_IMG) bs=512 count=20160	
	dd if=$(BIN_DIR)/$(OS_BIN) of=$(BIN_DIR)/$(OS_IMG) conv=notrunc

all-debug: O := -O0
all-debug: CFLAGS := -g -c -std=gnu99 -ffreestanding $(O) $(W) -fomit-frame-pointer
all-debug: LDFLAGS := -ffreestanding $(O) -fno-pie -fno-stack-protector -nostdlib -nostdinc -lgcc
all-debug: clean $(BIN_DIR)/$(OS_BIN)
	@i686-elf-objdump -D $(BIN_DIR)/$(OS_ELF) > dump
	@xxd $(BIN_DIR)/$(OS_ELF) > dump.hex

all: $(BIN_DIR)/$(OS_BIN)

clean:
	@rm -rf $(BUILD_DIR)

run: $(BIN_DIR)/$(OS_IMG) 
	@qemu-system-i386 -d int -no-shutdown -no-reboot -m 128M -drive file=$(BIN_DIR)/$(OS_IMG),index=0,media=disk,format=raw -monitor telnet::$(QEMU_MON_PORT),server,nowait &
	@sleep 1
	@telnet 127.0.0.1 $(QEMU_MON_PORT)

debug-bochs: all
	@bochs -q -f bochs.cfg

debug-qemu: all-debug
	@objcopy --only-keep-debug $(BIN_DIR)/$(OS_ELF) $(BUILD_DIR)/kernel.dbg
	@qemu-system-i386 -s -S -drive file=$(BIN_DIR)/$(OS_BIN),index=0,media=disk,format=raw &
	@gdb -s $(BUILD_DIR)/kernel.dbg \
		-ex "target remote localhost:1234" \
		-ex "set disassembly-flavor intel" \
		-ex "set architecture i8086" \
		-ex "continue" \
		-ex "quit"