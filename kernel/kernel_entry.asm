global _start;
[bits 32]

section .text

_start:
    [extern kernel_main] ; Define calling point. Must have same name as loader.c 'main' function
    jmp kernel_main ; Calls the C function. The linker will know where it is placed in memory
    jmp $
