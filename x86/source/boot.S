#include "os.h"

.section .data

.section .text
    .global _start       
    
    .code16          
    _start:   
        jmp $0, $offest

        offest:                      
            movw $0, %ax          

            movw %ax, %ds
            movw %ax, %ss
            movw %ax, %es
            movw %ax, %fs
            movw %ax, %gs

            movl $_start, %esp       

        boot_load:
            movw $protected_mode_start, %bx       
            movw $0x2, %cx          
            movw $0x240, %ax         
            int $0x13
            jc boot_load             

            cli                      
            lgdt gdt_desc          

            movl %cr0, %eax          
            orl $0x1, %eax           
            movl %eax, %cr0          

            jmp $KERNEL_CODE_SEG, $protected_mode_start

        boot_sig:
            .org 0x1fe
            .byte 0x55, 0xaa       

        .code32  
        protected_mode_start:
            movl $pg_dir, %eax
            movl %eax, %cr3

            movw $KERNEL_DATA_SEG, %ax
            movw %ax, %ds             
            movw %ax, %ss            
            movw %ax, %es              
            movw %ax, %fs            
            movw %ax, %gs              


            movl $_start, %esp  

            movl %cr4, %eax
            orl $(1 << 4), %eax
            movl %eax, %cr4

            movl %cr0, %eax
            orl $(1 << 31), %eax
            movl %eax, %cr0

            jmp . 

        gdt_desc: 
            .word (256*8) - 1        
            .long gdt_table         
