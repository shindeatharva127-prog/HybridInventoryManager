#pragma once

#include <vector>
#include "inventory.h"   /* Item struct + C backend */

enum class SortOrder { ById, ByName };

class InventoryManager {
public:
    void addItem();
    void viewItem();
    void updateItem();
    void deleteItem();
    void listItems(SortOrder order = SortOrder::ById);
};
