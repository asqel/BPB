ENTRY(_start)

SECTIONS {
    . = 0x8000;

    .text ALIGN(4K) : {
		src/start.o (.text)
		*(.multiboot)
        *(.text*)
    }

    .rodata ALIGN(4K) : {
        *(.rodata*)
    }

    .data ALIGN(4K) : {
        *(.data*)
    }

    .bss ALIGN(4K) : {
        __bss_start = .;
        *(.bss*)
        *(COMMON)
        __bss_end = .;
    }

    /DISCARD/ : {
        *(.eh_frame)
        *(.comment)
    }
}

