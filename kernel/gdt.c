#include <kernel.h>

// Structure d'un descripteur GDT
typedef struct {
    u16 limit_low;  // Limite (bits 0-15)
    u16 base_low;   // Base (bits 0-15)
    u8 base_middle; // Base (bits 16-23)
    u8 access;      // Octet d'accès
    u8 granularity; // Granularité et limite (bits 16-19)
    u8 base_high;   // Base (bits 24-31)
} __attribute__((packed)) GDTEntry;

// Structure pour la table GDT
typedef struct {
    u16 limit;      // Taille de la GDT moins 1
    u32 base;       // Adresse de la première entrée de la GDT
} __attribute__((packed)) GDTPtr;

// Nombre d'entrées dans notre GDT
#define GDT_ENTRIES 3

// GDT statique et pointeur vers la GDT
GDTEntry gdt[GDT_ENTRIES];
GDTPtr gdt_ptr;

// Fonction pour initialiser un descripteur GDT
void set_gdt_entry(int num, u32 base, u32 limit, u8 access, u8 granularity) {
    gdt[num].base_low = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= granularity & 0xF0;
    gdt[num].access = access;
}

// Fonction pour initialiser la GDT
void init_gdt() {
    // Configuration du pointeur GDT
    gdt_ptr.limit = sizeof(GDTEntry) * GDT_ENTRIES - 1;
    gdt_ptr.base = (u32)&gdt;

    // Entrée nulle (obligatoire)
    set_gdt_entry(0, 0, 0, 0, 0);

    // Segment de code : base=0, limite=4GB, exécutable, en mode protégé
    set_gdt_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Segment de données : base=0, limite=4GB, lisible/écrivable
    set_gdt_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Charger la GDT en ASM
    asm volatile (
        "lgdt (%0)\n"         // Charger la GDT
        "mov $0x10, %%ax\n"   // Charger le segment de données (sélecteur 0x10)
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        "ljmp $0x08, $.next\n"    // Saut far pour changer le segment de code (sélecteur 0x08)
        ".next:\n"
        : : "r" (&gdt_ptr)
    );
}
