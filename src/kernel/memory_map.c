#include "../../headers/kernel/memory_map.h"
#include "../../headers/drivers/io.h"
#include "../../headers/kernel/utils.h"

uint32_t bitmap[PAGE_NUMBER];

void init_bitmap()
{
   mem_map* mmap = (mem_map*)MEM_MAP_LOCATION;

char tmp[20];
    //initialize bitmap with PAGE_ALLOCATED
   for (size_t i = 4000; i < 4500; ++i)
   {
    // int_to_ascii(i, tmp);
    // terminal_writestring(tmp);
    // terminal_writestring("\n");
        // asm volatile("xchg %bx, %bx");
        bitmap[i] = 0x08080808;
   }

    int_to_ascii(mmap->size, tmp);
    terminal_writestring(tmp);
    terminal_writestring("\n");
//    for (size_t i = 0; i < (size_t)mmap->size; ++i)
//     {
//         terminal_writestring("Base: ");
//         int_to_ascii(mmap->entries[i].base, tmp);
//         terminal_writestring(tmp);
//         terminal_writestring("\n");
//         terminal_writestring("Length: ");
//          int_to_ascii(mmap->entries[i].size, tmp);
//         terminal_writestring(tmp);
//         terminal_writestring("\n");
//         terminal_writestring("Type: ");
//          int_to_ascii(mmap->entries[i].type, tmp);
//         terminal_writestring(tmp);
//         terminal_writestring("\n");
//         if (mmap->entries[i].type == E820_FREE)
//         {
//             uint64_t first_page = mmap->entries[i].base + PAGE_SIZE - mmap->entries[i].base % PAGE_SIZE; // we make sure not to cross reserved memory, so we cut down to the first 4kib align
//             uint64_t last_page = mmap->entries[i].base + mmap->entries[i].size - (mmap->entries[i].base + mmap->entries[i].size) % PAGE_SIZE; // same as above
//             for (uint64_t current_page = first_page; current_page <= last_page; ++current_page)
//             {
//                 size_t row = current_page / PAGE_SIZE / 8;
//                 uint32_t col = current_page / PAGE_SIZE % 8;
//                 bitmap[row] = bitmap[row] ^ (0x1 << col); // flip the bit at col in uint32_t numbered row
//             }
//         }
//     }
}

