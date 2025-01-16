#include <pci.h>
#include <oeuf.h>
#include <kernel.h>


pci_device_t *pcis = NULL;
int pcis_len = 0;

u16 pci_read_config(u32 bus, u32 slot, u32 func, u32 offset) {
    u32 address;
    u32 lbus  = (u32)bus;
    u32 lslot = (u32)slot;
    u32 lfunc = (u32)func;
    u32 tmp = 0;

    // Create configuration address as per Figure 1
    address = (u32)((lbus << 16) | (lslot << 11) |
              (lfunc << 8) | (offset & 0xFC) | ((u32)0x80000000));

    // Write out the address
    port_write_u32(0xCF8, address);
    // Read in the data
    // (offset & 2) * 8) = 0 will choose the first word of the 32-bit register
    tmp = (u16)((port_read_u32(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;

}

void pci_get_ids(pci_device_t *pci) {
	pci->vendor_id = pci_read_config(pci->bus, pci->slot, pci->function, 0);
	if (pci->vendor_id == 0xFFFF)
		return ;
	pci->device_id = pci_read_config(pci->bus, pci->slot, pci->function, 2);
}

void pci_add_device(pci_device_t *pci) {
	pcis = realloc(pcis, (pcis_len + 1) * sizeof(pci_device_t));
	pcis[pcis_len] = *pci;
	pcis_len++;
}

void pci_get_bars(pci_device_t *pci) {
    for (int i = 0; i < 6; i++) {
        u32 bar = pci_read_config(pci->bus, pci->slot, pci->function, 0x10 + (i * 4));
        pci->bar[i] = bar;
        pci->bar_is_mem[i] = 0 == (bar & 0x1);
    }
}

void pci_get_class(pci_device_t *pci) {
    u16 upper_word = pci_read_config(pci->bus, pci->slot, pci->function, 0x0A); // Bits 31-16
    u16 lower_word = pci_read_config(pci->bus, pci->slot, pci->function, 0x08); // Bits 15-0

    u32 class_info = ((u32)upper_word << 16) | lower_word; // Combine en un u32

    pci->class_id = (class_info >> 24) & 0xFF;       // Bits 31-2`4

    u16 class_code = pci_read_config(pci->bus, pci->slot, pci->function, 0x08);
    pci->subclass_id = (class_code >> 8) & 0xFF;
    pci->prog_if = class_code & 0xFF;
}

void pci_init() {
	for (int i = 0; i < 256; i++) {
		for (int k = 0; k < 16; k++) {
			for (int l = 0; l < 8; l++) {
				pci_device_t pci = {0};
				pci.bus = i;
				pci.slot = k;
				pci.function = l;
				pci_get_ids(&pci);
				if (pci.vendor_id == 0xFFFF)
					continue;
				pci_get_bars(&pci);
				pci_get_class(&pci);
				pci_add_device(&pci);
                pci.interrupt_line = (u8)pci_read_config(pci.bus, pci.slot, pci.function, 0x3C);
                pci.interrupt_pin = (u8)pci_read_config(pci.bus, pci.slot, pci.function, 0x3D);
			}
		}
    }
}

char *get_vendor_name(u16 id) {
    switch (id) {
        case 0x1022: return "AMD";
        case 0x10EC: return "Realtek";
        case 0x1234: return "Bochs/QEMU";
        case 0x1AF4: return "VirtIO";
        case 0x1B21: return "ASMedia";
        case 0x1B36: return "Red Hat";
        case 0x1D6B: return "Linux Foundation";
        case 0x8086: return "Intel";
        default:     return "Unknown";
    }
}

char *get_class_name(u16 id) {
    switch (id) {
        case 0x00: return "Unknown";
        case 0x01: return "Mass Storage Controller";
        case 0x02: return "Network Controller";
        case 0x03: return "Display Controller";
        case 0x04: return "Multimedia Controller";
        case 0x05: return "Memory Controller";
        case 0x06: return "Bridge Device";
        case 0x07: return "Simple Communication Controller";
        case 0x08: return "Base System Peripheral";
        case 0x09: return "Input Device";
        case 0x0A: return "Docking Station";
        case 0x0B: return "Processor";
        case 0x0C: return "Serial Bus Controller";
        case 0x0D: return "Wireless Controller";
        case 0x0E: return "Intelligent Controller";
        case 0x0F: return "Satellite Communication Controller";
        case 0x10: return "Encryption Controller";
        case 0x11: return "Signal Processing Controller";
        case 0x12: return "Processing Accelerator";
        case 0x13: return "Non-Essential Instrumentation";
        case 0x40: return "Co-Processor";
        case 0xFF: return "Unassigned Class";
        default:   return "Unknown";
    }
}

void pci_print() {
	fprintf(serialout, "vendor class [vendor_id device_id class_id function]\n");
	fprintf(serialout, "    bar0 bar1 bar2 bar3 bar4 bar5\n\n");
	for (int i = 0; i < pcis_len; i++) {
		char *vendor_name = get_vendor_name(pcis[i].vendor_id);
		char *class_name = get_class_name(pcis[i].class_id);
		fprintf(serialout, "%s %s [%x %x %x %d]\n", vendor_name, class_name,pcis[i].vendor_id, pcis[i].device_id, pcis[i].class_id, pcis[i].function);
		fprintf(serialout, "    %x %x %x %x %x %x\n", pcis[i].bar[0], pcis[i].bar[1], pcis[i].bar[2], pcis[i].bar[3], pcis[i].bar[4], pcis[i].bar[5]);
	}
}