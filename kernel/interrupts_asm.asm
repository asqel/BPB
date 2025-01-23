
[extern isr_handler_c]
[extern irq_handler_c]

; Common ISR code
isr_common:
    ; 1. Save CPU state
    pusha
	xor eax, eax
    mov ax, ds
    push eax        ; save the data segment descriptor
    mov ax, 0x10    ; kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp        ; registers_t *r

    cld ; clear direction flag
    call isr_handler_c

    ; 3. Restore state
    pop eax ; remove push esp (register_t *r)
    pop eax ; remove push eax (data segment descriptor)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8
    iret        ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

; Common IRQ code. Identical to ISR code except for the 'call'
; and the 'pop ebx'
irq_common:
    pusha
	xor eax, eax
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp

    cld
    call irq_handler_c

    pop ebx         ; Different than the ISR code
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    iret


%macro ISR_NO_ERR 1
	global _isr%1
_isr%1:
	push byte 0
	push byte %1
	jmp isr_common

%endmacro

%macro ISR_ERR 1
	global _isr%1
_isr%1:
	push byte %1
	jmp isr_common

%endmacro

%macro IRQ 1
	global _irq%1
_irq%1:
	push byte %1
	push byte %1+32
	jmp irq_common

%endmacro


ISR_NO_ERR 0  ; div by 0
ISR_NO_ERR 1  ; debug
ISR_NO_ERR 2  ; nmi
ISR_NO_ERR 3  ; int3 (breakpoint debug)
ISR_NO_ERR 4  ; overflow
ISR_NO_ERR 5  ; bounds
ISR_NO_ERR 6  ; invalid opcode
ISR_NO_ERR 7  ; coprocessor not available (ex: no fpu)
ISR_ERR 8     ; double fault
ISR_NO_ERR 9  ; coprocessor segment overrun
ISR_ERR 10    ; bad tss
ISR_ERR 11    ; segment not present
ISR_ERR 12    ; stack fault (segfault with stack operation)
ISR_ERR 13    ; general protection fault
ISR_ERR 14    ; page fault
ISR_NO_ERR 15 ; reserved
ISR_NO_ERR 16 ; fpu error
ISR_NO_ERR 17 ; alignment check
ISR_NO_ERR 18 ; machine check
ISR_NO_ERR 19 ; simd fpu error
; 0x14-0x1F: reserved (20-31)
ISR_NO_ERR 20
ISR_NO_ERR 21
ISR_NO_ERR 22
ISR_NO_ERR 23
ISR_NO_ERR 24
ISR_NO_ERR 25
ISR_NO_ERR 26
ISR_NO_ERR 27
ISR_NO_ERR 28
ISR_NO_ERR 29
ISR_NO_ERR 30
ISR_NO_ERR 31

;IRQS
IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15
