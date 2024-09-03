OUTPUT_FORMAT("elf32-i386");
ENTRY(_start);
SECTIONS
{
    . = 0x7C00;
    .boottext : AT(0x7C00) SUBALIGN(0) {
        build/obj/boot.o(.text*);
    }
    .bootdata : SUBALIGN(0) {
        build/obj/boot.o(.rodata*);
        build/obj/boot.o(.data);
    }

    .text : SUBALIGN(16) {
        *(.text*);
    }
    .data : SUBALIGN(16) {
        *(.data);
    }
    .rodata : SUBALIGN(16) {
        *(.rodata*);
    }
    .bss : SUBALIGN(16) {
        *(.COMMON);
        *(.bss)
    }

    /DISCARD/ : {
        *(.eh_frame);
        *(.comment);
        *(.note.gnu.build-id);
    }
}