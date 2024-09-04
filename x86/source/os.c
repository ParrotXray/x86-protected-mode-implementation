#include "os.h"
#include "page.h"
#include "tty.h"
#include <stdint.h>

struct GDTDescriptor {
    uint16_t segment_limit;
    uint16_t base_address;
    uint16_t base_address_low; 
    uint16_t base_address_high;
} __attribute__((aligned(8)));

struct IDTDescriptor {
    uint16_t offset_low;
    uint16_t segment_selector;
    uint16_t attr; 
    uint16_t offset_high;
} __attribute__((aligned(8)));

uint8_t map_phy_buffer[PG_4K] PG_4K_Aligned = {0x36};

static uint32_t page_table[PG_SIZE] PG_4K_Aligned = {0x80};

uint32_t pg_dir[PG_SIZE] PG_4K_Aligned = {
    [0] = (0) | PDE_P | PDE_PS | PDE_W | PDE_U,	  
};

struct GDTDescriptor gdt_table[256] = {
    [KERNEL_CODE_SEG / 8] = {0xffff, 0x0000, 0x9a00, 0x00cf},
    [KERNEL_DATA_SEG / 8] = {0xffff, 0x0000, 0x9200, 0x00cf}
};

struct IDTDescriptor idt_table[256] = {};

void outb(uint8_t data, uint16_t port) {
	__asm__ __volatile__("outb %[v], %[p]" : : [p]"d" (port), [v]"a" (data));
}

void timer_init (void);

void _os_init(void) {
    outb(0x11, 0x20);
    outb(0x11, 0xa0);

    outb(0x20, 0x21);
    outb(0x28, 0xa1);

    outb((1 << 2), 0x21);
    outb(2, 0xa1);

    outb(0x1, 0x21);
    outb(0x1, 0xa1);

    outb(0xfe, 0x21);
    outb(0xff, 0xa1);

    int time_value = 1193180;
    outb(0x36, 0x43);
    outb((uint8_t)time_value, 0x40);
    outb((time_value >> 8), 0x40);

    idt_table[0x20].offset_high = (uint32_t)timer_init >> 16;
    idt_table[0x20].offset_low = (uint32_t)timer_init & 0xffff;
    idt_table[0x20].segment_selector = KERNEL_CODE_SEG;
    idt_table[0x20].attr = 0x8E00;

    pg_dir[MAP_ADDR >> 22] = (uint32_t)page_table | PDE_P | PDE_W | PDE_U;
    page_table[(MAP_ADDR >> 12) & 0x3FF] = (uint32_t)map_phy_buffer | PDE_P | PDE_W | PDE_U;
}

void _os_main(void* info_table) {
    //TODO
    tty_set_theme(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
    tty_put_str("Hello \t world!\n");
}