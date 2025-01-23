ENTRY(_start)

SECTIONS {
    kernel_start = .;

    . = 0x10000;


    .multiboot : {
        *(.multiboot)
    }

    .text ALIGN (0x1000): {
        *(.text)
    }

    .rodata : {
        *(.rodata*)
    }

    .bss : {
        *(COMMON)
        *(.bss)
    }

    .data : {
        *(.data)
    }

    kernel_end = .;
}
