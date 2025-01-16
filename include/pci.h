#ifndef PCI_H
#define PCI_H

#include <kernel.h>

typedef struct {
	int vendor_id;
	int device_id;
	int bus;
	int slot;
	int function;
	u8 bar_is_mem[6];
	u32 bar[6];
	u32 class_id;
	u8 subclass_id;
	u8 prog_if;
	u8 interrupt_line;
	u8 interrupt_pin;
} pci_device_t;

#endif