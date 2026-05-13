#include "inventory.h"

#include <stdio.h>
#include <string.h>

/* ---------- internal helpers ---------- */

/** Return the byte offset of record index i. */
static long record_offset(int i)
{
    return (long)i * (long)sizeof(Item);
}

/**
 * Count the total number of records (including deleted ones) in the file.
 * Returns -1 on error.
 */
static int total_records(FILE *fp)
{
    if (fseek(fp, 0L, SEEK_END) != 0) return -1;
    long size = ftell(fp);
    if (size < 0) return -1;
    return (int)(size / (long)sizeof(Item));
}

/* ---------- public API ---------- */

int add_item(const Item *item)
{
    if (!item) return 0;

    FILE *fp = fopen(DB_FILE, "ab+");  /* create if missing, append */
    if (!fp) {
        /* Try plain write if file does not exist yet */
        fp = fopen(DB_FILE, "wb");
        if (!fp) return 0;
    }
    fclose(fp);

    /* Re-open for read+write so we can scan for duplicates */
    fp = fopen(DB_FILE, "rb+");
    if (!fp) {
        /* File might be brand new — create it */
        fp = fopen(DB_FILE, "wb");
        if (!fp) return 0;
        size_t written = fwrite(item, sizeof(Item), 1, fp);
        fclose(fp);
        return (written == 1) ? 1 : 0;
    }

    int n = total_records(fp);
    if (n < 0) { fclose(fp); return 0; }

    /* Scan for duplicate ID */
    rewind(fp);
    Item tmp;
    for (int i = 0; i < n; i++) {
        if (fread(&tmp, sizeof(Item), 1, fp) != 1) { fclose(fp); return 0; }
        if (!tmp.is_deleted && tmp.id == item->id) {
            fclose(fp);
            return 0;   /* duplicate */
        }
    }

    /* Append at end */
    if (fseek(fp, 0L, SEEK_END) != 0) { fclose(fp); return 0; }
    size_t written = fwrite(item, sizeof(Item), 1, fp);
    fclose(fp);
    return (written == 1) ? 1 : 0;
}

int get_item(int id, Item *out)
{
    if (!out) return 0;

    FILE *fp = fopen(DB_FILE, "rb");
    if (!fp) return 0;

    int n = total_records(fp);
    if (n < 0) { fclose(fp); return 0; }

    rewind(fp);
    Item tmp;
    for (int i = 0; i < n; i++) {
        if (fread(&tmp, sizeof(Item), 1, fp) != 1) break;
        if (!tmp.is_deleted && tmp.id == id) {
            *out = tmp;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int update_item(int id, const Item *updated)
{
    if (!updated) return 0;

    FILE *fp = fopen(DB_FILE, "rb+");
    if (!fp) return 0;

    int n = total_records(fp);
    if (n < 0) { fclose(fp); return 0; }

    rewind(fp);
    Item tmp;
    for (int i = 0; i < n; i++) {
        if (fread(&tmp, sizeof(Item), 1, fp) != 1) { fclose(fp); return 0; }
        if (!tmp.is_deleted && tmp.id == id) {
            /* Seek back to the start of this record and overwrite */
            if (fseek(fp, record_offset(i), SEEK_SET) != 0) { fclose(fp); return 0; }
            size_t written = fwrite(updated, sizeof(Item), 1, fp);
            fclose(fp);
            return (written == 1) ? 1 : 0;
        }
    }
    fclose(fp);
    return 0;
}

int delete_item(int id)
{
    FILE *fp = fopen(DB_FILE, "rb+");
    if (!fp) return 0;

    int n = total_records(fp);
    if (n < 0) { fclose(fp); return 0; }

    rewind(fp);
    Item tmp;
    for (int i = 0; i < n; i++) {
        if (fread(&tmp, sizeof(Item), 1, fp) != 1) { fclose(fp); return 0; }
        if (!tmp.is_deleted && tmp.id == id) {
            tmp.is_deleted = 1;
            if (fseek(fp, record_offset(i), SEEK_SET) != 0) { fclose(fp); return 0; }
            size_t written = fwrite(&tmp, sizeof(Item), 1, fp);
            fclose(fp);
            return (written == 1) ? 1 : 0;
        }
    }
    fclose(fp);
    return 0;
}

int list_items(Item *buffer, int max_items)
{
    if (!buffer || max_items <= 0) return 0;

    FILE *fp = fopen(DB_FILE, "rb");
    if (!fp) return 0;

    int n = total_records(fp);
    if (n < 0) { fclose(fp); return 0; }

    rewind(fp);
    int count = 0;
    Item tmp;
    for (int i = 0; i < n && count < max_items; i++) {
        if (fread(&tmp, sizeof(Item), 1, fp) != 1) break;
        if (!tmp.is_deleted) {
            buffer[count++] = tmp;
        }
    }
    fclose(fp);
    return count;
}
