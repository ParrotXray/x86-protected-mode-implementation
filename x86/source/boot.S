#include "os.h"

.section .data

.section .text
    .global _start, timer_init
    .code16          
    _start:   
        ljmp $0, $offest

    offest:                      
        movw $0, %ax          
        movw %ax, %ds
        movw %ax, %ss
        movw %ax, %es
        movw %ax, %fs
        movw %ax, %gs
        movl $_start, %esp 

    boot_load:
        movw $_protected_mode_start, %bx       
        movw $0x2, %cx          
        movw $0x240, %ax  
        movw $0x80, %dx       
        int $0x13
        jc boot_load     

        cli                      
        lgdt gdt_desc
        lidt idt_desc         
        movl $1, %eax
        lmsw %ax	     
        jmp $KERNEL_CODE_SEG, $_protected_mode_start

    boot_sig:
        .org 0x1fe
        .word 0xAA55   

    .code32  
    _protected_mode_start:
        movw $KERNEL_DATA_SEG, %ax
        movw %ax, %ds             
        movw %ax, %ss            
        movw %ax, %es              
        movw %ax, %fs            
        movw %ax, %gs              

        movl $_start, %esp  
        call _os_init

        movl $pg_dir, %eax
        movl %eax, %cr3

        movl %cr4, %eax
        orl $(1 << 4), %eax
        movl %eax, %cr4

        movl %cr0, %eax
        orl $(1 << 31), %eax
        movl %eax, %cr0

        movw $TASK0_TSS_SEG, %ax
        ltr %ax

        pushl $APP_DATA_SEG
        pushl $task_0_dpl_3_stack + 1024
        pushl $0x202
        pushl $APP_CODE_SEG
        pushl $task_0_entry
        iret 

    task_0_entry:
        movw %ss, %ax
        movw %ax, %ds             
        movw %ax, %ss            
        movw %ax, %es              
        movw %ax, %fs            
        movw %ax, %gs              

        jmp task_0

    timer_init:
        pushw %ds 
        pusha

        movb $0x20, %al
        outb %al, $0x20

        movw $KERNEL_DATA_SEG, %ax
        movw %ax, %ds

        call task_sched
        popa
        popw %ds
        iret 


    gdt_desc: 
        .word (256*8) - 1        
        .long gdt_table         

    idt_desc: 
        .word (256*8) - 1        
        .long idt_table      

        
