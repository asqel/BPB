[org 0x7c00]
KERNEL_OFFSET equ 0x1000

    mov [BOOT_DRIVE], dl
    mov bp, 0x9000
    mov sp, bp

    call load_kernel
    call switch_to_32bit


; write to [es:bx] dh nbr of sector (drive dl)
disk_load:
    pusha

    mov ah, 0x02    ; read
    mov al, dh      ; nbr of sect
    mov ch, 0x00    ; cylinder i guess
    mov dl, [BOOT_DRIVE]
    mov dh, 0x00    ; head i guess

    int 0x13
    ; i dont care about erros 
    popa
    ret

%include "boot/gdt.asm"
%include "boot/switch_to_32bit.asm"

[bits 16]
load_kernel:
    mov bx, KERNEL_OFFSET
    mov dh, KERNEL_SIZE
    mov cl, 0x02    ; sector of start (1 is bootsector)
    call disk_load
    ret

[bits 32]
BEGIN_PM:
    jmp KERNEL_OFFSET ; that's so far
    jmp $


BOOT_DRIVE db 0

; magic things happen here woooooooooooooo
times 510 - ($-$$) db 0
dw 0xaa55

KERNEL_START:
incbin "kernel.bin"
KERNEL_END:

KERNEL_SIZE equ (KERNEL_END - KERNEL_START + 511) / 512