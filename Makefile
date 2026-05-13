# ─────────────────────────────────────────────────────────────────────────────
#  Inventory Manager — Makefile
#  C files compile as C (gcc), C++ files compile as C++ (g++), linked by g++.
# ─────────────────────────────────────────────────────────────────────────────

TARGET  := inventory

CC      := gcc
CXX     := g++

CFLAGS   := -std=c11   -Wall -Wextra -pedantic -Iinclude
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -Iinclude
LDFLAGS  :=

SRC_DIR := src
OBJ_DIR := build

C_SRCS   := $(SRC_DIR)/inventory.c
CPP_SRCS := $(SRC_DIR)/InventoryManager.cpp $(SRC_DIR)/main.cpp

C_OBJS   := $(patsubst $(SRC_DIR)/%.c,   $(OBJ_DIR)/%.o, $(C_SRCS))
CPP_OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CPP_SRCS))
OBJS     := $(C_OBJS) $(CPP_OBJS)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^
	@echo ""
	@echo "  Build successful → ./$(TARGET)"

# Compile C source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Compile C++ source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(TARGET) inventory.dat
