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

// void task_0 (void) {
//     int color = VGA_COLOR_GREEN;
//     tty_set_theme(color, VGA_COLOR_BLACK);
    
//     for(;;) {
//         // tty_put_str("task 0\n");
//         tty_set_theme(++color, VGA_COLOR_BLACK);
//     }
// }

// void task_1 (void) {
//     int color = VGA_COLOR_WHITE;
//     tty_set_theme(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
//     for(;;) {
//         // tty_put_str("task 1\n");
//         tty_set_theme(--color, VGA_COLOR_BLACK);
//     }
// }

void task_0 (void) {
    // 加上下面这句会跑飞
    // *(unsigned char *)MAP_ADDR = 0x1;

    uint8_t color = 0;
    for (;;) {
        color++;

        // CPL=3时，非特权级模式下，无法使用cli指令
        // __asm__ __volatile__("cli");
    }
} 

/**
 * @brief 任务1
 */
void task_1 (void) {
    uint8_t color = 0xff;
    for (;;) {
        color--;
    }
}

uint8_t map_phy_buffer[PG_4K] PG_4K_Aligned = {0x36};

static uint32_t page_table[PG_SIZE] PG_4K_Aligned = {1};

uint32_t pg_dir[PG_SIZE] PG_4K_Aligned = {
    [0] = (0) | PDE_P | PDE_PS | PDE_W | PDE_U,	  
};

uint32_t task_0_dpl_3_stack[PG_SIZE], task_1_dpl_3_stack[PG_SIZE], task_0_dpl_0_stack[PG_SIZE], task_1_dpl_0_stack[PG_SIZE]; 

uint32_t task0_tss[] = {
    // prelink, esp0, ss0, esp1, ss1, esp2, ss2
    0,  (uint32_t)task_0_dpl_0_stack + 4*1024, KERNEL_DATA_SEG , 0x0, 0x0, 0x0, 0x0,
    // cr3, eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi,
    (uint32_t)pg_dir,  (uint32_t)task_0, 0x202, 0xa, 0xc, 0xd, 0xb, (uint32_t)task_0_dpl_3_stack + 4*1024/* 栈 */, 0x1, 0x2, 0x3,
    // es, cs, ss, ds, fs, gs, ldt, iomap
    APP_DATA_SEG, APP_CODE_SEG, APP_DATA_SEG, APP_DATA_SEG, APP_DATA_SEG, APP_DATA_SEG, 0x0, 0x0,
};

uint32_t task1_tss[] = {
    // prelink, esp0, ss0, esp1, ss1, esp2, ss2
    0,  (uint32_t)task_1_dpl_0_stack + 4*1024, KERNEL_DATA_SEG , 0x0, 0x0, 0x0, 0x0,
    // cr3, eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi,
    (uint32_t)pg_dir,  (uint32_t)task_1, 0x202, 0xa, 0xc, 0xd, 0xb, (uint32_t)task_1_dpl_3_stack + 4*1024/* 栈 */, 0x1, 0x2, 0x3,
    // es, cs, ss, ds, fs, gs, ldt, iomap
    APP_DATA_SEG, APP_CODE_SEG, APP_DATA_SEG, APP_DATA_SEG, APP_DATA_SEG, APP_DATA_SEG, 0x0, 0x0,
};

struct GDTDescriptor gdt_table[256] = {
    [KERNEL_CODE_SEG / 8] = {0xffff, 0x0000, 0x9a00, 0x00cf},
    [KERNEL_DATA_SEG / 8] = {0xffff, 0x0000, 0x9200, 0x00cf},

    [APP_CODE_SEG / 8] = {0xffff, 0x0000, 0xfa00, 0x00cf},
    [APP_DATA_SEG / 8] = {0xffff, 0x0000, 0xf300, 0x00cf},

    [TASK0_TSS_SEG / 8] = {0x0068, 0, 0xe900, 0x0},
    [TASK1_TSS_SEG / 8] = {0x0068, 0, 0xe900, 0x0}

};

struct IDTDescriptor idt_table[256] = {};

void outb(uint8_t data, uint16_t port) {
	__asm__ __volatile__("outb %[v], %[p]" : : [p]"d" (port), [v]"a" (data));
}

void task_sched(void) {
    static int task_tss = TASK0_TSS_SEG;

    task_tss = (task_tss == TASK0_TSS_SEG) ? TASK1_TSS_SEG : TASK0_TSS_SEG;

    uint32_t addr[] = {0, task_tss};
    __asm__ __volatile__("ljmpl *(%[a])"::[a]"r"(addr));
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

    int time_value = (1193180 / 10);
    outb(0x36, 0x43);
    outb((uint8_t)time_value, 0x40);
    outb((time_value >> 8), 0x40);

    idt_table[0x20].offset_high = (uint32_t)timer_init >> 16;
    idt_table[0x20].offset_low = (uint32_t)timer_init & 0xffff;
    idt_table[0x20].segment_selector = KERNEL_CODE_SEG;
    idt_table[0x20].attr = 0x8E00;

    gdt_table[TASK0_TSS_SEG / 8].base_address = (uint16_t)(uint32_t)task0_tss;
    gdt_table[TASK1_TSS_SEG / 8].base_address = (uint16_t)(uint32_t)task1_tss;

    pg_dir[MAP_ADDR >> 22] = (uint32_t)page_table | PDE_P | PDE_W | PDE_U;
    page_table[(MAP_ADDR >> 12) & 0x3FF] = (uint32_t)map_phy_buffer | PDE_P | PDE_W | PDE_U;

}

