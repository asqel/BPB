#include <kernel.h>

typedef struct idt_entry {
    u16 base_low;      // Bits 0-15 de l'adresse de la fonction d'interruption
    u16 selector;      // Sélecteur de segment (généralement 0x08 pour le kernel)
    u8  zero;          // Doit être 0
    u8  flags;         // Drapeaux (type d'interruption, privilèges)
    u16 base_high;     // Bits 16-31 de l'adresse de la fonction d'interruption
} idt_entry;

typedef struct idt_ptr_t {
    u16 limit;         // Taille de l'IDT (en octets - 256 entrées = 0x7FF)
    u32 base;          // Adresse de base de l'IDT
} idt_ptr_t;

idt_entry idt[256];
idt_ptr_t idt_ptr;



void init_idt_entry(int num, u32 base, u16 selector, u8 flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector = selector;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

void memset(void *dst, int c, size_t len) {
	for (size_t i = 0; i < len; i++) {
		((u8 *)dst)[i] = c;
	}
}

void init_idt() {
    // Initialiser toutes les entrées de l'IDT à 0
    memset(&idt, 0, sizeof(idt));

    // Exemple : initialiser l'entrée pour une interruption (par exemple, #0 - Division par zéro)
    //init_idt_entry(0, (u32) isr0, 0x08, 0x8E);  // flags: 0x8E -> interruption de niveau privilégié

    // Remplir d'autres entrées pour les autres interruptions ou exceptions ici.

    // Charger l'IDT dans le registre IDTR
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base = (u32) &idt;
    asm volatile("lidt %0" : : "m" (idt_ptr));
	asm volatile ("sti");
}
