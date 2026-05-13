#ifndef INVENTORY_H
#define INVENTORY_H

#ifdef __cplusplus
extern "C" {
#endif

#define NAME_LEN 40
#define DB_FILE  "inventory.dat"

typedef struct {
    int   id;
    char  name[NAME_LEN];
    int   quantity;
    float price;
    int   is_deleted;   /* 0 = active, 1 = soft-deleted */
} Item;

/**
 * Add a new item.  Rejects duplicate IDs.
 * Returns 1 on success, 0 on failure.
 */
int add_item(const Item *item);

/**
 * Find one active item by id and copy it into *out.
 * Returns 1 on success, 0 if not found or deleted.
 */
int get_item(int id, Item *out);

/**
 * Overwrite the record that has the given id with *updated.
 * Returns 1 on success, 0 if not found.
 */
int update_item(int id, const Item *updated);

/**
 * Soft-delete the record with the given id (sets is_deleted = 1).
 * Returns 1 on success, 0 if not found.
 */
int delete_item(int id);

/**
 * Copy up to max_items active (non-deleted) items into buffer[].
 * Returns the number of items actually copied.
 */
int list_items(Item *buffer, int max_items);

#ifdef __cplusplus
}
#endif

#endif /* INVENTORY_H */
