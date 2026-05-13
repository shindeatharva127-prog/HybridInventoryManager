#include "InventoryManager.h"

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

/* ------------------------------------------------------------------ */
/*  Small terminal-colour helpers (ANSI, fall back gracefully)         */
/* ------------------------------------------------------------------ */
namespace {
    const char* RESET  = "\033[0m";
    const char* BOLD   = "\033[1m";
    const char* GREEN  = "\033[32m";
    const char* CYAN   = "\033[36m";
    const char* YELLOW = "\033[33m";
    const char* RED    = "\033[31m";

    void print_separator(char ch = '-', int width = 60) {
        std::cout << std::string(width, ch) << "\n";
    }

    void print_header(const char *title) {
        print_separator('=');
        std::cout << BOLD << CYAN << "  " << title << RESET << "\n";
        print_separator('=');
    }

    void print_item_row(const Item &it) {
        std::cout << std::left
                  << std::setw(6)  << it.id
                  << std::setw(42) << it.name
                  << std::setw(10) << it.quantity
                  << std::fixed << std::setprecision(2) << it.price
                  << "\n";
    }

    /* Read a positive integer; keep re-asking on bad input. */
    int read_positive_int(const char *prompt) {
        int v;
        while (true) {
            std::cout << prompt;
            if (std::cin >> v && v > 0) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return v;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << RED << "  Invalid — must be a positive integer.\n" << RESET;
        }
    }

    /* Read a non-negative integer. */
    int read_nonneg_int(const char *prompt) {
        int v;
        while (true) {
            std::cout << prompt;
            if (std::cin >> v && v >= 0) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return v;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << RED << "  Invalid — must be >= 0.\n" << RESET;
        }
    }

    /* Read a non-negative float. */
    float read_nonneg_float(const char *prompt) {
        float v;
        while (true) {
            std::cout << prompt;
            if (std::cin >> v && v >= 0.0f) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return v;
            }
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << RED << "  Invalid — must be >= 0.\n" << RESET;
        }
    }

    /* Read a non-empty string of at most max_len-1 chars. */
    void read_nonempty_string(const char *prompt, char *buf, int max_len) {
        std::string s;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, s);
            if (!s.empty()) {
                std::strncpy(buf, s.c_str(), max_len - 1);
                buf[max_len - 1] = '\0';
                return;
            }
            std::cout << RED << "  Name must not be empty.\n" << RESET;
        }
    }
} // anonymous namespace

/* ------------------------------------------------------------------ */
/*  InventoryManager implementation                                     */
/* ------------------------------------------------------------------ */

void InventoryManager::addItem()
{
    print_header("ADD ITEM");

    Item it{};
    it.id       = read_positive_int("  ID       : ");
    read_nonempty_string           ("  Name     : ", it.name, NAME_LEN);
    it.quantity = read_nonneg_int  ("  Quantity : ");
    it.price    = read_nonneg_float("  Price    : ");
    it.is_deleted = 0;

    if (add_item(&it)) {
        std::cout << GREEN << "\n  [OK] Item #" << it.id << " added.\n" << RESET;
    } else {
        std::cout << RED << "\n  [FAIL] Could not add item (duplicate ID or I/O error).\n" << RESET;
    }
}

void InventoryManager::viewItem()
{
    print_header("VIEW ITEM");
    int id = read_positive_int("  Enter ID : ");

    Item it{};
    if (get_item(id, &it)) {
        std::cout << "\n";
        std::cout << BOLD << "  ID       : " << RESET << it.id       << "\n";
        std::cout << BOLD << "  Name     : " << RESET << it.name     << "\n";
        std::cout << BOLD << "  Quantity : " << RESET << it.quantity  << "\n";
        std::cout << BOLD << "  Price    : " << RESET
                  << std::fixed << std::setprecision(2) << it.price   << "\n";
    } else {
        std::cout << RED << "\n  [FAIL] Item #" << id << " not found.\n" << RESET;
    }
}

void InventoryManager::updateItem()
{
    print_header("UPDATE ITEM");
    int id = read_positive_int("  Enter ID to update : ");

    Item existing{};
    if (!get_item(id, &existing)) {
        std::cout << RED << "\n  [FAIL] Item #" << id << " not found.\n" << RESET;
        return;
    }

    std::cout << "  (Leave blank / 0 to keep current value)\n\n";

    Item updated = existing;   /* start from existing values */

    /* Name */
    std::cout << "  New name [" << existing.name << "] : ";
    {
        std::string s;
        std::getline(std::cin, s);
        if (!s.empty()) {
            std::strncpy(updated.name, s.c_str(), NAME_LEN - 1);
            updated.name[NAME_LEN - 1] = '\0';
        }
    }

    /* Quantity */
    std::cout << "  New quantity [" << existing.quantity << "] : ";
    {
        std::string s;
        std::getline(std::cin, s);
        if (!s.empty()) {
            try {
                int q = std::stoi(s);
                if (q >= 0) updated.quantity = q;
                else std::cout << YELLOW << "  Kept old value (must be >= 0).\n" << RESET;
            } catch (...) {
                std::cout << YELLOW << "  Kept old value.\n" << RESET;
            }
        }
    }

    /* Price */
    std::cout << "  New price [" << std::fixed << std::setprecision(2)
              << existing.price << "] : ";
    {
        std::string s;
        std::getline(std::cin, s);
        if (!s.empty()) {
            try {
                float p = std::stof(s);
                if (p >= 0.0f) updated.price = p;
                else std::cout << YELLOW << "  Kept old value (must be >= 0).\n" << RESET;
            } catch (...) {
                std::cout << YELLOW << "  Kept old value.\n" << RESET;
            }
        }
    }

    if (update_item(id, &updated)) {
        std::cout << GREEN << "\n  [OK] Item #" << id << " updated.\n" << RESET;
    } else {
        std::cout << RED << "\n  [FAIL] Could not update item.\n" << RESET;
    }
}

void InventoryManager::deleteItem()
{
    print_header("DELETE ITEM");
    int id = read_positive_int("  Enter ID to delete : ");

    if (delete_item(id)) {
        std::cout << GREEN << "\n  [OK] Item #" << id << " deleted.\n" << RESET;
    } else {
        std::cout << RED << "\n  [FAIL] Item #" << id << " not found.\n" << RESET;
    }
}

void InventoryManager::listItems(SortOrder order)
{
    print_header("ALL ITEMS");

    /* Ask user for sort preference */
    std::cout << "  Sort by: [1] ID  [2] Name  (default: ID) : ";
    {
        std::string s;
        std::getline(std::cin, s);
        if (s == "2") order = SortOrder::ByName;
    }

    const int MAX = 1024;
    std::vector<Item> items(MAX);
    int n = list_items(items.data(), MAX);
    items.resize(n);

    if (n == 0) {
        std::cout << YELLOW << "  No items found.\n" << RESET;
        return;
    }

    /* STL sort */
    if (order == SortOrder::ByName) {
        std::sort(items.begin(), items.end(),
                  [](const Item &a, const Item &b) {
                      return std::strcmp(a.name, b.name) < 0;
                  });
    } else {
        std::sort(items.begin(), items.end(),
                  [](const Item &a, const Item &b) {
                      return a.id < b.id;
                  });
    }

    std::cout << "\n";
    std::cout << BOLD
              << std::left
              << std::setw(6)  << "ID"
              << std::setw(42) << "Name"
              << std::setw(10) << "Qty"
              << "Price"
              << RESET << "\n";
    print_separator();

    for (const auto &it : items) {
        print_item_row(it);
    }

    print_separator();
    std::cout << BOLD << "  Total active items: " << n << RESET << "\n";
}
