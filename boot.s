ORG 0x7C00
BITS 16

jmp boot_main
times 3-($-$$) db 0x90
	OEMname: db "mkfs.fat"
	bytesPerSector: dw 512
	sectPerCluster: db 1
	dw 1
	db 2
	dw 224
	dw 2880
	db 0xf0
	dw 9
	dw 18
	dw 2
	dd 0
	dd 0
	db 0
	db 0
	db 0x29
	dd 0x2d7e5a1a
	db "NO NAME    "
	db "FAT12   "
	dl_save: dw 0

boot_main:
	cli
	mov [dl_save], dl
	jmp 0:boot_main2 ; jmp because far jump is to big to be before part table
boot_main2:
	mov ax, 0x0000
	mov ss, ax
	mov sp, 0x7C00
	in al, 0x92
	or al, 2
	out 0x92, al

	mov ax, 0x1000
	mov es, ax
	mov ah, 2
	mov al, KERNEL_SIZE
	mov ch, 0
	mov cl, 2
	mov dh, 0
	mov bx, 0
	int 0x13

	lgdt [GDT_descriptor]
	mov eax, cr0
	or eax, 1    
	mov cr0, eax
	mov ax, 0x10   ; data segment = 3e entrée de GDT
	jmp 0x8:prot
prot:
BITS 32
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, 0x9FC00 ; stack en RAM sûre
	mov word [0x7c00 + 512], KERNEL_SIZE
	mov al, [dl_save]
	mov byte [0x7c00 + 512 + 2], al
	call 0x8:0x10000

GDT_start:
    dq 0x0000000000000000       ; Null descriptor
    dq 0x00CF9A000000FFFF       ; Code segment 32-bit, base=0, limit=4GB
    dq 0x00CF92000000FFFF       ; Data segment 32-bit, base=0, limit=4GB

GDT_descriptor:
    dw GDT_end - GDT_start - 1  ; limit (taille GDT - 1)
    dd GDT_start                ; base

GDT_end:


times 510 - ($ - $$) db 0
db 0x55, 0xaa

align 512, db 0
kernel_entry:
incbin "kernel.bin"

align 512, db 0
DISK_START:
times 512 db 0x42

KERNEL_SIZE equ (DISK_START - kernel_entry) / 512
