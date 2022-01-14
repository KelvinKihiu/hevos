#include "streamer.h"
#include "config.h"
#include "memory/heap/kheap.h"

#include <stdbool.h>


struct disk_stream* diskstreamer_new(int disk_id)
{
    struct disk* disk = disk_get(disk_id);
    if (!disk)
    {
        return 0;
    }

    struct disk_stream* streamer = kzalloc(sizeof(struct disk_stream));
    streamer->pos = 0;
    streamer->disk = disk;
    return streamer;
}

int diskstreamer_read(struct disk_stream* stream, void* out, int total)
{
    int sector = stream->pos / HEVOS_SECTOR_SIZE;
    int offset = stream->pos % HEVOS_SECTOR_SIZE;
    int total_to_read = total;
    bool overflow = (offset + total_to_read) >= HEVOS_SECTOR_SIZE;
    char buf[HEVOS_SECTOR_SIZE];

    if (overflow)
    {
        total_to_read -= (offset + total_to_read) - HEVOS_SECTOR_SIZE;
    }

    int res = disk_read_block(stream->disk, sector, 1, buf);
    if (res < 0)
    {
        goto out;
    }

    for (int i = 0; i < total_to_read; i++)
    {
        *(char*)out++ = buf[offset+i];
    }
    
    // Adjust the stream
    stream->pos += total_to_read;
    if (overflow)
    {
        res = diskstreamer_read(stream, out, total - total_to_read);
    }
    
out:
    return res;
}


int diskstreamer_write(struct disk_stream* stream, void* data, int total)
{
    int sector = stream->pos / HEVOS_SECTOR_SIZE;
    int offset = stream->pos % HEVOS_SECTOR_SIZE;
    int total_to_write = total;
    bool overflow = (offset + total_to_write) >= HEVOS_SECTOR_SIZE;

    if (overflow)
    {
        total_to_write -= (offset + total_to_write) - HEVOS_SECTOR_SIZE;
    }

    int res = disk_write_block(stream->disk, sector, 1, data);
    if (res < 0)
    {
        goto out;
    }
    
    // Adjust the stream
    stream->pos += total_to_write;
    if (overflow)
    {
        res = diskstreamer_write(stream, data + total_to_write, total - total_to_write);
    }
    
out:
    return res;
}


int diskstreamer_seek(struct disk_stream* stream, int pos)
{
    stream->pos = pos;
    return 0;
}


void diskstreamer_close(struct disk_stream* stream)
{
    kfree(stream);
}