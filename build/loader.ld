ENTRY(_start)
SECTIONS
{
	. = 0x1000;

	.text : {
		loader/loader_entry.o (.text) /* if not placed here it is not executed first*/
		*(.text)
	}

	.data : {
		*(.data)
	}

	.bss : {
		*(.bss)
		*(COMMON)
	}
}
