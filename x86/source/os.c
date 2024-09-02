#include "os.h"
#include "page.h"
#include <stdint.h>

struct GDTDescriptor {
    uint16_t segment_limit;
    uint16_t base_address;
    uint16_t base_address_low; 
    uint16_t base_address_high;
} __attribute__((aligned(8)));

uint32_t pg_dir[PG_SIZE] __attribute__((aligned(4096))) =  {
    [0] = (0) | PDE_P | PDE_PS | PDE_W | PDE_U,	  
};

struct GDTDescriptor gdt_table[256] = {
    [KERNEL_CODE_SEG / 8] = {0xffff, 0x0000, 0x9a00, 0x00cf},
    [KERNEL_DATA_SEG / 8] = {0xffff, 0x0000, 0x9200, 0x00cf}
};

