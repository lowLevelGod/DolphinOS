#include "../../headers/kernel/memory_map.h"
#include "../../headers/drivers/io.h"
#include "../../headers/kernel/utils.h"

const uint32_t higherHalfBase = 0xC0000000;

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t kernel_page_table[1024] __attribute__((aligned(4096)));
uint32_t identity_page_table[1024] __attribute__((aligned(4096)));
uint32_t test_page_table[1024] __attribute__((aligned(4096)));

extern void loadPageDirectory(uint32_t*);
extern void enablePaging();

void init_paging()
{
    //set each entry to not present
    for(size_t i = 0; i < 1024; i++)
    {
        // This sets the following flags to the pages:
        //   Supervisor: Only kernel-mode can access them
        //   Write Enabled: It can be both read from and written to
        //   Not Present: The page table is not present
        page_directory[i] = 0x00000002;
    }

    // holds the physical address where we want to start mapping these pages to.
    // in this case, we want to map these pages to the very beginning of memory.
    //we will fill all 1024 entries in the table, mapping 4 megabytes
    for(size_t i = 0; i < 1024; i++)
    {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes ;)
        test_page_table[i] = 0x00000002;
        identity_page_table[i] = (i * 0x1000) | 3;
        kernel_page_table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
    }

    // 768 is page directory entry for 0xC0000000 = 768 * 1024 * 4096
    uint32_t kernelpt = (uint32_t)&kernel_page_table;
    kernelpt -= higherHalfBase;
    uint32_t pagedir = (uint32_t)&page_directory;
    pagedir -= higherHalfBase;
    uint32_t identitypt = (uint32_t)&identity_page_table;
    identitypt -= higherHalfBase;

    page_directory[768] = kernelpt | 3;
    page_directory[0] = identitypt | 3; // we are keeping the identity mapping for now because of gdt wrong linear address
    
    kprintf("Page dir address: %d\n", pagedir);

    loadPageDirectory(pagedir);
    enablePaging();
}

void createPageDirEntry(uint32_t index)
{
    // TODO
    // testing purposes only
    uint32_t testpt = (uint32_t)&test_page_table;
    testpt -= higherHalfBase;
    page_directory[index] = testpt | 3;
}

void createPageTableEntry(uint32_t index)
{
    uint32_t page = kAllocPhysical();
    kprintf("Page allocated: %d\n", page);
    test_page_table[index] = (page * 0x1000) | 3;
}

uint32_t bitmap[PAGE_NUMBER];
struct PageStack pageStack;

static uint32_t last_usable_page = PAGE_NUMBER;

// static const kernel_first_page = 0x10;
// static const kernel_last_page = 0x14;

static inline size_t getBitmapRow(uint32_t current_page)
{
    return current_page / 32;
}

static inline uint32_t getBitmapCol(uint32_t current_page)
{
    return current_page % 32;
}

void init_pmm()
{
    // empty page stack
    pageStack.sz = 0;

    mem_map *mmap = (mem_map *)MEM_MAP_LOCATION;

    // initialize bitmap with PAGE_ALLOCATED
    for (size_t i = 0; i < PAGE_NUMBER; ++i)
    {
        bitmap[i] = 0xFFFFFFFF;
    }

    // kprintf("Size: %d\n", mmap->size);
    for (size_t i = 0; i < (size_t)mmap->size; ++i)
    {
            kprintf("Base:%d Size: %d Type: %d\n", mmap->entries[i].base_low, mmap->entries[i].size_low, mmap->entries[i].type);
        if (mmap->entries[i].type == E820_FREE)
        {
            uint32_t first_page = (mmap->entries[i].base_low + PAGE_SIZE - 
            mmap->entries[i].base_low % PAGE_SIZE) / PAGE_SIZE; // we make sure not to cross reserved memory, so we cut down to the first 4kib align

            uint32_t last_page = (mmap->entries[i].base_low + mmap->entries[i].size_low - 
            (mmap->entries[i].base_low + mmap->entries[i].size_low) % PAGE_SIZE - PAGE_SIZE) / PAGE_SIZE; // same as above
            
            kprintf("Range %d |---> %d\n", first_page, last_page);

            for (uint32_t current_page = first_page; current_page <= last_page; ++current_page)
            {
                size_t row = getBitmapRow(current_page);
                uint32_t col = getBitmapCol(current_page);
                bitmap[row] = bitmap[row] ^ (0x1 << col); // flip the bit at col in uint32_t numbered row
            }

            if (last_page < UINT32_MAX && last_page > last_usable_page)
                last_usable_page = last_page;
        }
    }

    // RESERVE THE FIRST 4MB FOR KERNEL
    for (uint32_t page = 0; page < 0x400; ++page) // 0x400 = 1024
    {
        size_t row = getBitmapRow(page);
        uint32_t col = getBitmapCol(page);
        bitmap[row] = bitmap[row] | (0x1 << col);
    }
}

int getPageStatus(uint64_t page)
{
    size_t row = getBitmapRow(page);
    uint32_t col = getBitmapCol(page);

    return bitmap[row] & (0x1 << col);
}

void setPageStatus(uint64_t page, enum PHYSICAL_PAGE_STATUS status)
{
    size_t row = getBitmapRow(page);
    uint32_t col = getBitmapCol(page);

    if (status)
    {
        bitmap[row] = bitmap[row] | (0x1 << col);
    }else
    {
        bitmap[row] = bitmap[row] & ~(0x1 << col);
    }
}

void kAllocPhysicalBunch()
{
    for (size_t i = 0; i <= last_usable_page; ++i)
    {
        if (getPageStatus(i) == PAGE_FREE)
        {
            pageStack.stack[pageStack.sz] = i;
            ++pageStack.sz;
        }

        if (pageStack.sz == PAGE_STACK_SIZE)
            break;
    }
}

uint64_t kAllocPhysical()
{
    if (pageStack.sz == 0)
    {
        kAllocPhysicalBunch();
    }

    --pageStack.sz;
    setPageStatus(pageStack.stack[pageStack.sz], PAGE_ALLOCATED);
    return pageStack.stack[pageStack.sz];
}

void kFreePhysical(uint64_t page)
{
    setPageStatus(page, PAGE_FREE);
    if (pageStack.sz < PAGE_STACK_SIZE)
    {
        pageStack.stack[pageStack.sz] = page;
        ++pageStack.sz;
    }
}

void mapPage(uint32_t vaddr)
{
    uint32_t pdirIndex = (vaddr >> 22) & 0x3ff;
    uint32_t ptableIndex = (vaddr >> 12) & 0x3ff;

    if (!(page_directory[pdirIndex] & 0x1))
    {
        // kprintf("Page Table: %d not found !\n", pdirIndex);
        createPageDirEntry(pdirIndex);
    }
    if (!(test_page_table[ptableIndex] & 0x1))
    {
        // kprintf("Page: %d not found !\n", ptableIndex);
        createPageTableEntry(ptableIndex);
    }

    uint32_t pagedir = (uint32_t)&page_directory;
    pagedir -= higherHalfBase;
    // kprintf("Page dir address: %d\n", pagedir);
    loadPageDirectory(pagedir);

    // for (int i = 0; i < 1024; ++i)
    //     if (page_directory[i] & 0x1)
    //     kprintf("Page dir entry %d has value %d\n", i, page_directory[i]);
}
