
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


%macro ISR_NEG 2
    global _isr%1
_isr%1:
    push byte 0
    push byte %2
    jmp isr_common

%endmacro

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


; 48-255: user defined
ISR_NO_ERR 48
ISR_NO_ERR 49
ISR_NO_ERR 50
ISR_NO_ERR 51
ISR_NO_ERR 52
ISR_NO_ERR 53
ISR_NO_ERR 54
ISR_NO_ERR 55
ISR_NO_ERR 56
ISR_NO_ERR 57
ISR_NO_ERR 58
ISR_NO_ERR 59
ISR_NO_ERR 60
ISR_NO_ERR 61
ISR_NO_ERR 62
ISR_NO_ERR 63
ISR_NO_ERR 64
ISR_NO_ERR 65
ISR_NO_ERR 66
ISR_NO_ERR 67
ISR_NO_ERR 68
ISR_NO_ERR 69
ISR_NO_ERR 70
ISR_NO_ERR 71
ISR_NO_ERR 72
ISR_NO_ERR 73
ISR_NO_ERR 74
ISR_NO_ERR 75
ISR_NO_ERR 76
ISR_NO_ERR 77
ISR_NO_ERR 78
ISR_NO_ERR 79
ISR_NO_ERR 80
ISR_NO_ERR 81
ISR_NO_ERR 82
ISR_NO_ERR 83
ISR_NO_ERR 84
ISR_NO_ERR 85
ISR_NO_ERR 86
ISR_NO_ERR 87
ISR_NO_ERR 88
ISR_NO_ERR 89
ISR_NO_ERR 90
ISR_NO_ERR 91
ISR_NO_ERR 92
ISR_NO_ERR 93
ISR_NO_ERR 94
ISR_NO_ERR 95
ISR_NO_ERR 96
ISR_NO_ERR 97
ISR_NO_ERR 98
ISR_NO_ERR 99
ISR_NO_ERR 100
ISR_NO_ERR 101
ISR_NO_ERR 102
ISR_NO_ERR 103
ISR_NO_ERR 104
ISR_NO_ERR 105
ISR_NO_ERR 106
ISR_NO_ERR 107
ISR_NO_ERR 108
ISR_NO_ERR 109
ISR_NO_ERR 110
ISR_NO_ERR 111
ISR_NO_ERR 112
ISR_NO_ERR 113
ISR_NO_ERR 114
ISR_NO_ERR 115
ISR_NO_ERR 116
ISR_NO_ERR 117
ISR_NO_ERR 118
ISR_NO_ERR 119
ISR_NO_ERR 120
ISR_NO_ERR 121
ISR_NO_ERR 122
ISR_NO_ERR 123
ISR_NO_ERR 124
ISR_NO_ERR 125
ISR_NO_ERR 126
ISR_NO_ERR 127
ISR_NEG 128, -128
ISR_NEG 129, -127
ISR_NEG 130, -126
ISR_NEG 131, -125
ISR_NEG 132, -124
ISR_NEG 133, -123
ISR_NEG 134, -122
ISR_NEG 135, -121
ISR_NEG 136, -120
ISR_NEG 137, -119
ISR_NEG 138, -118
ISR_NEG 139, -117
ISR_NEG 140, -116
ISR_NEG 141, -115
ISR_NEG 142, -114
ISR_NEG 143, -113
ISR_NEG 144, -112
ISR_NEG 145, -111
ISR_NEG 146, -110
ISR_NEG 147, -109
ISR_NEG 148, -108
ISR_NEG 149, -107
ISR_NEG 150, -106
ISR_NEG 151, -105
ISR_NEG 152, -104
ISR_NEG 153, -103
ISR_NEG 154, -102
ISR_NEG 155, -101
ISR_NEG 156, -100
ISR_NEG 157, -99
ISR_NEG 158, -98
ISR_NEG 159, -97
ISR_NEG 160, -96
ISR_NEG 161, -95
ISR_NEG 162, -94
ISR_NEG 163, -93
ISR_NEG 164, -92
ISR_NEG 165, -91
ISR_NEG 166, -90
ISR_NEG 167, -89
ISR_NEG 168, -88
ISR_NEG 169, -87
ISR_NEG 170, -86
ISR_NEG 171, -85
ISR_NEG 172, -84
ISR_NEG 173, -83
ISR_NEG 174, -82
ISR_NEG 175, -81
ISR_NEG 176, -80
ISR_NEG 177, -79
ISR_NEG 178, -78
ISR_NEG 179, -77
ISR_NEG 180, -76
ISR_NEG 181, -75
ISR_NEG 182, -74
ISR_NEG 183, -73
ISR_NEG 184, -72
ISR_NEG 185, -71
ISR_NEG 186, -70
ISR_NEG 187, -69
ISR_NEG 188, -68
ISR_NEG 189, -67
ISR_NEG 190, -66
ISR_NEG 191, -65
ISR_NEG 192, -64
ISR_NEG 193, -63
ISR_NEG 194, -62
ISR_NEG 195, -61
ISR_NEG 196, -60
ISR_NEG 197, -59
ISR_NEG 198, -58
ISR_NEG 199, -57
ISR_NEG 200, -56
ISR_NEG 201, -55
ISR_NEG 202, -54
ISR_NEG 203, -53
ISR_NEG 204, -52
ISR_NEG 205, -51
ISR_NEG 206, -50
ISR_NEG 207, -49
ISR_NEG 208, -48
ISR_NEG 209, -47
ISR_NEG 210, -46
ISR_NEG 211, -45
ISR_NEG 212, -44
ISR_NEG 213, -43
ISR_NEG 214, -42
ISR_NEG 215, -41
ISR_NEG 216, -40
ISR_NEG 217, -39
ISR_NEG 218, -38
ISR_NEG 219, -37
ISR_NEG 220, -36
ISR_NEG 221, -35
ISR_NEG 222, -34
ISR_NEG 223, -33
ISR_NEG 224, -32
ISR_NEG 225, -31
ISR_NEG 226, -30
ISR_NEG 227, -29
ISR_NEG 228, -28
ISR_NEG 229, -27
ISR_NEG 230, -26
ISR_NEG 231, -25
ISR_NEG 232, -24
ISR_NEG 233, -23
ISR_NEG 234, -22
ISR_NEG 235, -21
ISR_NEG 236, -20
ISR_NEG 237, -19
ISR_NEG 238, -18
ISR_NEG 239, -17
ISR_NEG 240, -16
ISR_NEG 241, -15
ISR_NEG 242, -14
ISR_NEG 243, -13
ISR_NEG 244, -12
ISR_NEG 245, -11
ISR_NEG 246, -10
ISR_NEG 247, -9
ISR_NEG 248, -8
ISR_NEG 249, -7
ISR_NEG 250, -6
ISR_NEG 251, -5
ISR_NEG 252, -4
ISR_NEG 253, -3
ISR_NEG 254, -2
ISR_NEG 255, -1