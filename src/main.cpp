#include "InventoryManager.h"

#include <iostream>
#include <limits>
#include <string>

namespace {
    const char* RESET  = "\033[0m";
    const char* BOLD   = "\033[1m";
    const char* CYAN   = "\033[36m";
    const char* GREEN  = "\033[32m";
    const char* YELLOW = "\033[33m";

    void show_menu() {
        std::cout << "\n";
        std::cout << BOLD << CYAN
                  << "╔══════════════════════════════╗\n"
                  << "║    INVENTORY MANAGER v1.0    ║\n"
                  << "╠══════════════════════════════╣\n"
                  << RESET;
        std::cout << BOLD
                  << "║  " << RESET << " 1. Add item              " << BOLD << "  ║\n"
                  << "║  " << RESET << " 2. View item by ID       " << BOLD << "  ║\n"
                  << "║  " << RESET << " 3. Update item           " << BOLD << "  ║\n"
                  << "║  " << RESET << " 4. Delete item           " << BOLD << "  ║\n"
                  << "║  " << RESET << " 5. List all items        " << BOLD << "  ║\n"
                  << "║  " << RESET << " 6. Exit                  " << BOLD << "  ║\n"
                  << CYAN
                  << "╚══════════════════════════════╝\n"
                  << RESET;
        std::cout << BOLD << "Choice: " << RESET;
    }
}

int main()
{
    InventoryManager mgr;

    while (true) {
        show_menu();

        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << YELLOW << "  Please enter a number 1-6.\n" << RESET;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "\n";
        switch (choice) {
            case 1: mgr.addItem();    break;
            case 2: mgr.viewItem();   break;
            case 3: mgr.updateItem(); break;
            case 4: mgr.deleteItem(); break;
            case 5: mgr.listItems();  break;
            case 6:
                std::cout << GREEN << "  Goodbye!\n\n" << RESET;
                return 0;
            default:
                std::cout << YELLOW << "  Invalid choice — enter 1 to 6.\n" << RESET;
        }
    }
}
