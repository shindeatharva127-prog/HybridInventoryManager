cmake_minimum_required(VERSION 3.14)
project(InventoryManager LANGUAGES C CXX)

set(CMAKE_C_STANDARD   11)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)

# C object library (compiled as C)
add_library(inventory_c OBJECT src/inventory.c)
set_target_properties(inventory_c PROPERTIES LINKER_LANGUAGE C)

# C++ executable
add_executable(inventory
    src/InventoryManager.cpp
    src/main.cpp
    $<TARGET_OBJECTS:inventory_c>
)

target_include_directories(inventory PRIVATE include)

# Warnings
target_compile_options(inventory_c  PRIVATE -Wall -Wextra)
target_compile_options(inventory    PRIVATE -Wall -Wextra)
