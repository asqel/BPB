ENTRY(_start)

SECTIONS {
    /* Le kernel est chargé par ton bootloader à l’adresse 0x8000 */
    . = 0x8000;

    /* Code (instructions) */
    .text ALIGN(4K) :
		start.o (.text)
		*(.multiboot)
        *(.text*)
    }

    /* Données en lecture seule */
    .rodata ALIGN(4K) : {
        *(.rodata*)
    }

    /* Données initialisées (.data) */
    .data ALIGN(4K) : {
        *(.data*)
    }

    /* Données non initialisées (.bss) */
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

