#include "../../headers/kernel/memory_map.h"
#include "../../headers/drivers/io.h"
#include "../../headers/kernel/utils.h"

uint32_t bitmap[PAGE_NUMBER];
static uint32_t last_usable_page = PAGE_NUMBER;

static const kernel_first_page = 0x10;
static const kernel_last_page = 0x14;

static inline size_t getBitmapRow(uint64_t current_page)
{
    return current_page / PAGE_SIZE / 8;
}

static inline uint32_t getBitmapCol(uint64_t current_page)
{
    return current_page / PAGE_SIZE % 8;
}

void init_bitmap()
{
    mem_map *mmap = (mem_map *)MEM_MAP_LOCATION;

    // initialize bitmap with PAGE_ALLOCATED
    for (size_t i = 0; i < PAGE_NUMBER; ++i)
    {
        bitmap[i] = 0xFFFFFFFF;
    }

    for (size_t i = 0; i < (size_t)mmap->size; ++i)
    {
        if (mmap->entries[i].type == E820_FREE)
        {
            uint64_t first_page = mmap->entries[i].base + PAGE_SIZE - 
            mmap->entries[i].base % PAGE_SIZE; // we make sure not to cross reserved memory, so we cut down to the first 4kib align

            uint64_t last_page = mmap->entries[i].base + mmap->entries[i].size - 
            (mmap->entries[i].base + mmap->entries[i].size) % PAGE_SIZE; // same as above

            for (uint64_t current_page = first_page; current_page <= last_page; ++current_page)
            {
                size_t row = current_page / PAGE_SIZE / 8;
                uint32_t col = current_page / PAGE_SIZE % 8;
                bitmap[row] = bitmap[row] ^ (0x1 << col); // flip the bit at col in uint32_t numbered row
            }

            if (last_page < UINT32_MAX && last_page > last_usable_page)
                last_usable_page = last_page;
        }
    }

    //also mark kernel space as PAGE ALLOCATED 
    for (uint64_t page = kernel_first_page; page <= kernel_last_page; ++page)
    {
        size_t row = getBitmapRow(page);
        uint32_t col = getBitmapCol(page);
        bitmap[row] = bitmap[row] | (0x1 << col);
    }
    //TODO RESERVE AND MAP THE FIRST 1MB, SO I DON'T HAVE TO DEAL WITH STACK
}
