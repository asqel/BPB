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

extern void _isr0();
extern void _isr1();
extern void _isr2();
extern void _isr3();
extern void _isr4();
extern void _isr5();
extern void _isr6();
extern void _isr7();
extern void _isr8();
extern void _isr9();
extern void _isr10();
extern void _isr11();
extern void _isr12();
extern void _isr13();
extern void _isr14();
extern void _isr15();
extern void _isr16();
extern void _isr17();
extern void _isr18();
extern void _isr19();
extern void _isr20();
extern void _isr21();
extern void _isr22();
extern void _isr23();
extern void _isr24();
extern void _isr25();
extern void _isr26();
extern void _isr27();
extern void _isr28();
extern void _isr29();
extern void _isr30();
extern void _isr31();

extern void _irq0();
extern void _irq1();
extern void _irq2();
extern void _irq3();
extern void _irq4();
extern void _irq5();
extern void _irq6();
extern void _irq7();
extern void _irq8();
extern void _irq9();
extern void _irq10();
extern void _irq11();
extern void _irq12();
extern void _irq13();
extern void _irq14();
extern void _irq15();


void init_idt_entry(int num, void *base) {
    idt[num].base_low = ((u32)base )& 0xFFFF;
    idt[num].base_high = (((u32)base) >> 16) & 0xFFFF;
    idt[num].selector = 0x08;
    idt[num].zero = 0;
    idt[num].flags = 0x8e;
}

static void memset(void *dst, int c, size_t len) {
	for (size_t i = 0; i < len; i++) {
		((u8 *)dst)[i] = c;
	}
}

#define PIC1 0x20       // Adresse du PIC maître (Command)
#define PIC2 0xA0       // Adresse du PIC esclave (Command)
#define PIC1_DATA 0x21  // Données du PIC maître
#define PIC2_DATA 0xA1  // Données du PIC esclave
#define ICW1_INIT 0x10  // Initialisation requise
#define ICW1_ICW4 0x01  // ICW4 sera envoyé
#define ICW4_8086 0x01  // Mode 8086/88 (pas en mode MCS-80/85)

static inline void io_wait(void) {
    // Effectue une opération I/O sur un port inutile
    asm volatile ("outb %%al, $0x80" : : "a"(0));
}


void remap_pic() {
    // Sauvegarder les masques d'interruptions actuels
    u8 master_mask = port_read_u8(PIC1_DATA);
    u8 slave_mask = port_read_u8(PIC2_DATA);

    // send ICW1 - start initialization
    port_write_u8(PIC1, ICW1_INIT | ICW1_ICW4);
    io_wait(); // Petite attente pour stabiliser les opérations
    port_write_u8(PIC2, ICW1_INIT | ICW1_ICW4);
    io_wait();

    // send ICW2 - remap offsets
    port_write_u8(PIC1_DATA, 0x20); //master offset 0x20-0x27
    io_wait();
	port_write_u8(PIC2_DATA, 0x28); // slave offset 0x28-0x2F
    io_wait();

    // send ICW3 - master / slave wiring
    port_write_u8(PIC1_DATA, 0x04);
    io_wait();
    port_write_u8(PIC2_DATA, 0x02);
    io_wait();

    // send ICW4 - 8086 mode
    port_write_u8(PIC1_DATA, ICW4_8086);
    io_wait();
    port_write_u8(PIC2_DATA, ICW4_8086);
    io_wait();

    // Restore saved masks
    port_write_u8(PIC1_DATA, master_mask);
    port_write_u8(PIC2_DATA, slave_mask);
}

void init_idt() {
	asm volatile ("cli");
    // Initialiser toutes les entrées de l'IDT à 0
    memset(&idt, 0, sizeof(idt));

	remap_pic();

	init_idt_entry(0, _isr0);
	init_idt_entry(1, _isr1);
	init_idt_entry(2, _isr2);
	init_idt_entry(3, _isr3);
	init_idt_entry(4, _isr4);
	init_idt_entry(5, _isr5);
	init_idt_entry(6, _isr6);
	init_idt_entry(7, _isr7);
	init_idt_entry(8, _isr8);
	init_idt_entry(9, _isr9);
	init_idt_entry(10, _isr10);
	init_idt_entry(11, _isr11);
	init_idt_entry(12, _isr12);
	init_idt_entry(13, _isr13);
	init_idt_entry(14, _isr14);
	init_idt_entry(15, _isr15);
	init_idt_entry(16, _isr16);
	init_idt_entry(17, _isr17);
	init_idt_entry(18, _isr18);
	init_idt_entry(19, _isr19);
	init_idt_entry(20, _isr20);
	init_idt_entry(21, _isr21);
	init_idt_entry(22, _isr22);
	init_idt_entry(23, _isr23);
	init_idt_entry(24, _isr24);
	init_idt_entry(25, _isr25);
	init_idt_entry(26, _isr26);
	init_idt_entry(27, _isr27);
	init_idt_entry(28, _isr28);
	init_idt_entry(29, _isr29);
	init_idt_entry(30, _isr30);
	init_idt_entry(31, _isr31);

	init_idt_entry(32, _irq0);
	init_idt_entry(33, _irq1);
	init_idt_entry(34, _irq2);
	init_idt_entry(35, _irq3);
	init_idt_entry(36, _irq4);
	init_idt_entry(37, _irq5);
	init_idt_entry(38, _irq6);
	init_idt_entry(39, _irq7);
	init_idt_entry(40, _irq8);
	init_idt_entry(41, _irq9);
	init_idt_entry(42, _irq10);
	init_idt_entry(43, _irq11);
	init_idt_entry(44, _irq12);
	init_idt_entry(45, _irq13);
	init_idt_entry(46, _irq14);
	init_idt_entry(47, _irq15);


    // Charger l'IDT dans le registre IDTR
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base = (u32) &idt;
    asm volatile("lidt %0" : : "m" (idt_ptr));
	asm volatile ("sti");
}
