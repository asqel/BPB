[extern kernel_main]
[global _start]

section .text

_start:
    call kernel_main ; Calls the C function. The linker will know where it is placed in memory
    jmp $
