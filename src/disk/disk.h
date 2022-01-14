#ifndef DISK_H
#define DISK_H

#include "fs/file.h"

typedef unsigned int HEVOS_DISK_TYPE;

#define HEVOS_DISK_TYPE_REAL 0

struct disk
{
    HEVOS_DISK_TYPE type;
    int sector_size;

    int id;

    struct filesystem* filesystem;

    void* fs_private;
};

int disk_search_and_init();
struct disk* disk_get(int index);
int disk_read_block(struct disk* idisk, unsigned int lba, int total, void* buf);
int disk_write_block(struct disk* idisk, unsigned int lba, int total, void* buf);

#endif