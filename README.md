<<<<<<< HEAD
# Inventory Manager

A console-based inventory CRUD app with a **C data layer** (binary file I/O via
`fread`/`fwrite`/`fseek`) and a **C++ UI layer** (`InventoryManager` class, `std::vector`,
`std::sort`).

---

## File Structure

```
inventory_project/
├── include/
│   ├── inventory.h          # C struct + extern-C function declarations
│   └── InventoryManager.h   # C++ class declaration
├── src/
│   ├── inventory.c          # C backend (binary file storage)
│   ├── InventoryManager.cpp # C++ class implementation (menu actions)
│   └── main.cpp             # Entry point + menu loop
├── Makefile
├── CMakeLists.txt
└── README.md
```

---

## Build & Run

### Using Make (recommended)

```bash
# From the project root
make            # builds ./inventory
./inventory     # run the app
make clean      # remove build artefacts and inventory.dat
```

### Using CMake

```bash
cmake -B build_cmake
cmake --build build_cmake
./build_cmake/inventory
```

> **Requirements:** GCC ≥ 7 (C11) and G++ ≥ 7 (C++17), or Clang equivalents.

---

## Usage

The interactive menu presents six options:

```
1. Add item
2. View item by ID
3. Update item
4. Delete item
5. List all items
6. Exit
```

All data is persisted in **`inventory.dat`** (binary, same directory as the
executable).  The file is created automatically on the first `Add item`.

---

## Design Notes

| Concern | Layer | Technology |
|---|---|---|
| Persistent storage | C (`inventory.c`) | `fread` / `fwrite` / `fseek` on a flat binary file |
| Soft delete | C flag | `is_deleted` field; records are never physically removed |
| Listing & sorting | C++ (`InventoryManager`) | `std::vector<Item>` + `std::sort` |
| Input validation | C++ | loops that re-ask on bad input |

---

## Test Cases

1. **Persistence across restart**
   - Add items #1 "Widget" qty 10 price 2.99, #2 "Gadget" qty 5 price 9.99, #3 "Doohickey" qty 20 price 0.49.
   - Exit, relaunch → *List all* shows all three items.
   - ✅ Pass

2. **Duplicate ID rejection**
   - Add item #1 "Widget" (already exists).
   - App prints `[FAIL] Could not add item (duplicate ID or I/O error)`.
   - ✅ Pass

3. **Update persists across restart**
   - Update item #2: change price to 14.99.
   - Exit, relaunch → *View item 2* shows price 14.99.
   - ✅ Pass

4. **Soft delete hides item**
   - Delete item #3.
   - *List all* no longer shows #3; *View item 3* prints `[FAIL] Item #3 not found`.
   - ✅ Pass

5. **Input validation**
   - Attempt to add item with ID = -5 → re-prompted.
   - Attempt quantity = -1 → re-prompted.
   - Attempt empty name → re-prompted.
   - All invalid inputs produce an error message and ask again without crashing.
   - ✅ Pass
=======
# HybridInventoryManager
A hybrid Inventory Management System developed using C and C++.
The project combines low-level binary file operations in C with object-oriented programming and STL features in C++. 
Supports persistent data storage and CRUD operations including add, update, delete, search, and listing of inventory items.
>>>>>>> ce72a75562a22d69b9051928e4c317791370f038
