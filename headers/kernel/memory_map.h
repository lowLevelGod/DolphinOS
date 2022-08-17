#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H
#include <stdint.h>
#include <stddef.h>

#define MEM_MAP_SIZE 128
#define MEM_MAP_LOCATION 0xb000
#define E820_FREE 0x1
#define PAGE_SIZE 4096
#define PAGE_NUMBER 32768
#define PAGE_STACK_SIZE 100

struct PageStack
{
    uint64_t stack[PAGE_STACK_SIZE];
    size_t sz;
};

enum PHYSICAL_PAGE_STATUS
{
    PAGE_FREE = 0,
    PAGE_ALLOCATED = 1,
};

typedef struct __attribute__((packed))
{
    uint32_t base_low;
    uint32_t base_high;
    uint32_t size_low;
    uint32_t size_high;
    uint32_t type;
    uint32_t alignment;
} mem_entry_t;

typedef struct __attribute__((packed))
{
    uint32_t size;
    mem_entry_t entries[MEM_MAP_SIZE];
} mem_map;

uint64_t kAllocPhysical();
void createPageDirEntry(uint32_t);
void mapPage(uint32_t);
void init_paging();
void init_pmm();
int getPageStatus(uint64_t);
void setPageStatus(uint64_t, enum PHYSICAL_PAGE_STATUS);

#endif