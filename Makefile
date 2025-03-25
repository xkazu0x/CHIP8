CC := g++
CFLAGS := -std=c++17 -g -Wall -Wextra -Werror -Wpadded -O0 -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-write-strings
DEFINES := -DEXCALIBUR_INTERNAL=1 -DEXCALIBUR_DEBUG=1
INCLUDES := -I.
LIBS := -lkernel32 -luser32 -lgdi32 -lwinmm

SRC_DIR := src
BIN_DIR := bin

TARGET := chip8.exe

all: $(BIN_DIR) $(BIN_DIR)/$(TARGET)

$(BIN_DIR):
	mkdir $@

$(BIN_DIR)/$(TARGET): $(SRC_DIR)/chip8.cpp
	$(CC) $(CFLAGS) -mconsole -o $@ $^ $(DEFINES) $(INCLUDES) $(LIBS)

run: all
	$(BIN_DIR)/$(TARGET)

remake: clean all

clean:
	rmdir /s /q $(BIN_DIR)

asm:
	objdump -D $(BIN_DIR)/$(TARGET)
