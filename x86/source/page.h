#ifndef PAGE_H
#define PAGE_H

#define PG_SIZE             1024
#define PG_4K               4096
#define MAP_ADDR            (0x80000000)

#define PG_4K_Aligned       __attribute__((aligned(PG_4K)))

#define PDE_P			    (1 << 0)
#define PDE_W			    (1 << 1)
#define PDE_U			    (1 << 2)
#define PDE_PS			    (1 << 7)

#endif // PAGE_H