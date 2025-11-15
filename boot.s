%define FREE_SPACE 0xf000

ORG 0x7C00
BITS 16

; Main entry point where BIOS leaves us.

jmp load_disk               ; Some BIOS' may load us at 0x0000:0x7C00 while other may load us at 0x07C0:0x0000.
; Do a far jump to fix this issue, and reload CS to 0x0000.
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

load_disk:
	mov ax, 0
	mov es, ax
	mov ah, 2
	mov al, 100
	mov ch, 0
	mov cl, 2
	mov dh, 0
	mov bx, 0x7c00 + 512
	int 0x13

jmp 0x0000:Main
; Pad out file.
times 510 - ($-$$) db 0
dw 0xAA55

Main:
.FlushCS:   
    xor ax, ax

    ; Set up segment registers.
    mov ss, ax
    ; Set up stack so that it starts below Main.
    mov sp, Main
    
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    cld

    call CheckCPU                     ; Check whether we support Long Mode or not.
    jc .NoLongMode

    ; Point edi to a free space bracket.
    mov edi, FREE_SPACE
    ; Switch to Long Mode.
    jmp SwitchToLongMode


BITS 64
.Long:
	push DISK_START
    jmp 0x8000
	jmp .Long


BITS 16

.NoLongMode:
    mov si, NoLongMode
    call Print

.Die:
    hlt
    jmp .Die


%include "LongModeDirectly.asm"
BITS 16


NoLongMode db "ERROR: CPU does not support long mode.", 0x0A, 0x0D, 0


; Checks whether CPU supports long mode or not.

; Returns with carry set if CPU doesn't support long mode.

CheckCPU:
    ; Check whether CPUID is supported or not.
    pushfd                            ; Get flags in EAX register.
    
    pop eax
    mov ecx, eax  
    xor eax, 0x200000 
    push eax 
    popfd

    pushfd 
    pop eax
    xor eax, ecx
    shr eax, 21 
    and eax, 1                        ; Check whether bit 21 is set or not. If EAX now contains 0, CPUID isn't supported.
    push ecx
    popfd 

    test eax, eax
    jz .NoLongMode
    
    mov eax, 0x80000000   
    cpuid                 
    
    cmp eax, 0x80000001               ; Check whether extended function 0x80000001 is available are not.
    jb .NoLongMode                    ; If not, long mode not supported.

    mov eax, 0x80000001  
    cpuid                 
    test edx, 1 << 29                 ; Test if the LM-bit, is set or not.
    jz .NoLongMode                    ; If not Long mode not supported.

    ret

.NoLongMode:
    stc
    ret


; Prints out a message using the BIOS.

; es:si    Address of ASCIIZ string to print.




Print:
    pushad
.PrintLoop:
    lodsb                             ; Load the value at [@es:@si] in @al.
    test al, al                       ; If AL is the terminator character, stop printing.
    je .PrintDone                  	
    mov ah, 0x0E	
    int 0x10
    jmp .PrintLoop                    ; Loop till the null character not found.
	
.PrintDone:
    popad                             ; Pop all general purpose registers to save them.
	ret

times 1024 - ($-$$) db 0
kernel_entry:
incbin "kernel.bin"
align 512
DISK_START:
