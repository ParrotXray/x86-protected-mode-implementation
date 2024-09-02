#include "os.h"
#include "page.h"
#include <stdint.h>

struct GDTDescriptor {
    uint16_t segment_limit;
    uint16_t base_address;
    uint16_t base_address_low; 
    uint16_t base_address_high;
} __attribute__((aligned(8)));

uint8_t map_phy_buffer[PG_4K] __attribute__((aligned(PG_4K))) = {0x36};

static uint32_t page_table[PG_SIZE] __attribute__((aligned(PG_4K))) = {PDE_U};

uint32_t pg_dir[PG_SIZE] __attribute__((aligned(PG_4K))) = {
    [0] = (0) | PDE_P | PDE_PS | PDE_W | PDE_U,	  
};

struct GDTDescriptor gdt_table[256] = {
    [KERNEL_CODE_SEG / 8] = {0xffff, 0x0000, 0x9a00, 0x00cf},
    [KERNEL_DATA_SEG / 8] = {0xffff, 0x0000, 0x9200, 0x00cf}
};


void _os_init(void) {
    pg_dir[MAP_ADDR >> 22] = (uint32_t)page_table | PDE_P | PDE_W | PDE_U;
    pg_dir[(MAP_ADDR >> 12) & 0x3FF] = (uint32_t)map_phy_buffer | PDE_P | PDE_W | PDE_U;
}