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

boot_main:
	cli
	; reset segment
	mov ax, 0
	mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
	mov [dl_save], dl
	; reset disk
	mov ax, 0
	mov dl, [dl_save]
	int 0x13

	; load kernel to phys 0x10000
	mov ax, 0x1000
	mov es, ax
	mov bx, 0
	mov ah, 2
	mov al, KERNEL_SIZE
	mov ch, 0
	mov cl, 2
	mov dl, [dl_save]
	int 0x13

	; set up stack
	mov ax, 0
	mov ss, ax
	mov sp, 0x9000
	
	;write data for the kernel
	mov al, byte [dl_save]
	mov byte [0x7c00 + 512], al
	mov word [0x7c00 + 512 + 1], KERNEL_SIZE

	; switch to 32bit
	cli
		; A20
		in al, 0x92
		or al, 2
		out 0x92, al

		; gdt
		mov ax, 0
		mov ds, ax
		lgdt [gdt_descriptor]

		; set cr0 bit
		mov eax, cr0
		or eax, 1
		mov cr0, eax
		jmp 0x8:start_32
BITS 32
start_32:
	jmp 0x8:0x10000

BITS 16
	jmp $

data:
	dl_save: db 0

gdt_start:

gdt_null:
    dq 0

gdt_code:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x9a
    db 0xcf
    db 0x00

gdt_data:
    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 0x92
    db 0xcf
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510 - ($ - $$) db 0
db 0x55, 0xaa

align 512, db 0
kernel_entry:
incbin "kernel.bin"

align 512, db 0
DISK_START:
times 512 db 0x42

KERNEL_SIZE equ (DISK_START - kernel_entry) / 512
