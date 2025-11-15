[extern kernel_main]

_start:
    ; Charger les segments depuis la GDT
    mov ax, 0x10      ; data selector dans la GDT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 0x100000

    call kernel_main
	jmp $
