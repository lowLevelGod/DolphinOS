#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H
#include <stdint.h>
#include <stddef.h>

#define MEM_MAP_SIZE 128
#define MEM_MAP_LOCATION 0x9000
#define E820_FREE 0x1
#define PAGE_SIZE 4096
#define PAGE_NUMBER 32768
#define PAGE_ALLOCATED 1
#define PAGE_FREE 0

typedef struct __attribute__((packed))
{
    uint64_t base;
    // uint32_t base_high;
    uint64_t size;
    // uint32_t size_high;
    uint32_t type;
    uint32_t alignment;
} mem_entry_t;

typedef struct __attribute__((packed))
{
    uint32_t size;
    mem_entry_t entries[MEM_MAP_SIZE];
} mem_map;

void init_bitmap();

#endif