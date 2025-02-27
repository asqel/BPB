#include <kernel.h>

typedef struct idt_entry {
    u16 base_low;      // Bits 0-15 de l'adresse de la fonction d'interruption
    u16 selector;      // Sélecteur de segment (généralement 0x08 pour le kernel)
    u8  zero;          // Doit être 0
    u8  flags;         // Drapeaux (type d'interruption, privilèges)
    u16 base_high;     // Bits 16-31 de l'adresse de la fonction d'interruption
} __attribute__((packed)) idt_entry;

typedef struct idt_ptr_t {
    u16 limit;         // Taille de l'IDT (en octets - 256 entrées = 0x7FF)
    u32 base;          // Adresse de base de l'IDT
} __attribute__((packed)) idt_ptr_t;

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

extern void _isr48();
extern void _isr49();
extern void _isr50();
extern void _isr51();
extern void _isr52();
extern void _isr53();
extern void _isr54();
extern void _isr55();
extern void _isr56();
extern void _isr57();
extern void _isr58();
extern void _isr59();
extern void _isr60();
extern void _isr61();
extern void _isr62();
extern void _isr63();
extern void _isr64();
extern void _isr65();
extern void _isr66();
extern void _isr67();
extern void _isr68();
extern void _isr69();
extern void _isr70();
extern void _isr71();
extern void _isr72();
extern void _isr73();
extern void _isr74();
extern void _isr75();
extern void _isr76();
extern void _isr77();
extern void _isr78();
extern void _isr79();
extern void _isr80();
extern void _isr81();
extern void _isr82();
extern void _isr83();
extern void _isr84();
extern void _isr85();
extern void _isr86();
extern void _isr87();
extern void _isr88();
extern void _isr89();
extern void _isr90();
extern void _isr91();
extern void _isr92();
extern void _isr93();
extern void _isr94();
extern void _isr95();
extern void _isr96();
extern void _isr97();
extern void _isr98();
extern void _isr99();
extern void _isr100();
extern void _isr101();
extern void _isr102();
extern void _isr103();
extern void _isr104();
extern void _isr105();
extern void _isr106();
extern void _isr107();
extern void _isr108();
extern void _isr109();
extern void _isr110();
extern void _isr111();
extern void _isr112();
extern void _isr113();
extern void _isr114();
extern void _isr115();
extern void _isr116();
extern void _isr117();
extern void _isr118();
extern void _isr119();
extern void _isr120();
extern void _isr121();
extern void _isr122();
extern void _isr123();
extern void _isr124();
extern void _isr125();
extern void _isr126();
extern void _isr127();
extern void _isr128();
extern void _isr129();
extern void _isr130();
extern void _isr131();
extern void _isr132();
extern void _isr133();
extern void _isr134();
extern void _isr135();
extern void _isr136();
extern void _isr137();
extern void _isr138();
extern void _isr139();
extern void _isr140();
extern void _isr141();
extern void _isr142();
extern void _isr143();
extern void _isr144();
extern void _isr145();
extern void _isr146();
extern void _isr147();
extern void _isr148();
extern void _isr149();
extern void _isr150();
extern void _isr151();
extern void _isr152();
extern void _isr153();
extern void _isr154();
extern void _isr155();
extern void _isr156();
extern void _isr157();
extern void _isr158();
extern void _isr159();
extern void _isr160();
extern void _isr161();
extern void _isr162();
extern void _isr163();
extern void _isr164();
extern void _isr165();
extern void _isr166();
extern void _isr167();
extern void _isr168();
extern void _isr169();
extern void _isr170();
extern void _isr171();
extern void _isr172();
extern void _isr173();
extern void _isr174();
extern void _isr175();
extern void _isr176();
extern void _isr177();
extern void _isr178();
extern void _isr179();
extern void _isr180();
extern void _isr181();
extern void _isr182();
extern void _isr183();
extern void _isr184();
extern void _isr185();
extern void _isr186();
extern void _isr187();
extern void _isr188();
extern void _isr189();
extern void _isr190();
extern void _isr191();
extern void _isr192();
extern void _isr193();
extern void _isr194();
extern void _isr195();
extern void _isr196();
extern void _isr197();
extern void _isr198();
extern void _isr199();
extern void _isr200();
extern void _isr201();
extern void _isr202();
extern void _isr203();
extern void _isr204();
extern void _isr205();
extern void _isr206();
extern void _isr207();
extern void _isr208();
extern void _isr209();
extern void _isr210();
extern void _isr211();
extern void _isr212();
extern void _isr213();
extern void _isr214();
extern void _isr215();
extern void _isr216();
extern void _isr217();
extern void _isr218();
extern void _isr219();
extern void _isr220();
extern void _isr221();
extern void _isr222();
extern void _isr223();
extern void _isr224();
extern void _isr225();
extern void _isr226();
extern void _isr227();
extern void _isr228();
extern void _isr229();
extern void _isr230();
extern void _isr231();
extern void _isr232();
extern void _isr233();
extern void _isr234();
extern void _isr235();
extern void _isr236();
extern void _isr237();
extern void _isr238();
extern void _isr239();
extern void _isr240();
extern void _isr241();
extern void _isr242();
extern void _isr243();
extern void _isr244();
extern void _isr245();
extern void _isr246();
extern void _isr247();
extern void _isr248();
extern void _isr249();
extern void _isr250();
extern void _isr251();
extern void _isr252();
extern void _isr253();
extern void _isr254();
extern void _isr255();


void init_idt_entry(int num, void *base) {
    idt[num].base_low = ((u32)base )& 0xFFFF;
    idt[num].base_high = (((u32)base) >> 16) & 0xFFFF;
    idt[num].selector = 0x08;
    idt[num].zero = 0;
    idt[num].flags = 0x8e;
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

	init_idt_entry(0, &_isr0);
	init_idt_entry(1, &_isr1);
	init_idt_entry(2, &_isr2);
	init_idt_entry(3, &_isr3);
	init_idt_entry(4, &_isr4);
	init_idt_entry(5, &_isr5);
	init_idt_entry(6, &_isr6);
	init_idt_entry(7, &_isr7);
	init_idt_entry(8, &_isr8);
	init_idt_entry(9, &_isr9);
	init_idt_entry(10, &_isr10);
	init_idt_entry(11, &_isr11);
	init_idt_entry(12, &_isr12);
	init_idt_entry(13, &_isr13);
	init_idt_entry(14, &_isr14);
	init_idt_entry(15, &_isr15);
	init_idt_entry(16, &_isr16);
	init_idt_entry(17, &_isr17);
	init_idt_entry(18, &_isr18);
	init_idt_entry(19, &_isr19);
	init_idt_entry(20, &_isr20);
	init_idt_entry(21, &_isr21);
	init_idt_entry(22, &_isr22);
	init_idt_entry(23, &_isr23);
	init_idt_entry(24, &_isr24);
	init_idt_entry(25, &_isr25);
	init_idt_entry(26, &_isr26);
	init_idt_entry(27, &_isr27);
	init_idt_entry(28, &_isr28);
	init_idt_entry(29, &_isr29);
	init_idt_entry(30, &_isr30);
	init_idt_entry(31, &_isr31);

	init_idt_entry(32, &_irq0);
	init_idt_entry(33, &_irq1);
	init_idt_entry(34, &_irq2);
	init_idt_entry(35, &_irq3);
	init_idt_entry(36, &_irq4);
	init_idt_entry(37, &_irq5);
	init_idt_entry(38, &_irq6);
	init_idt_entry(39, &_irq7);
	init_idt_entry(40, &_irq8);
	init_idt_entry(41, &_irq9);
	init_idt_entry(42, &_irq10);
	init_idt_entry(43, &_irq11);
	init_idt_entry(44, &_irq12);
	init_idt_entry(45, &_irq13);
	init_idt_entry(46, &_irq14);
	init_idt_entry(47, &_irq15);

	init_idt_entry(48, &_isr48);
	init_idt_entry(49, &_isr49);
	init_idt_entry(50, &_isr50);
	init_idt_entry(51, &_isr51);
	init_idt_entry(52, &_isr52);
	init_idt_entry(53, &_isr53);
	init_idt_entry(54, &_isr54);
	init_idt_entry(55, &_isr55);
	init_idt_entry(56, &_isr56);
	init_idt_entry(57, &_isr57);
	init_idt_entry(58, &_isr58);
	init_idt_entry(59, &_isr59);
	init_idt_entry(60, &_isr60);
	init_idt_entry(61, &_isr61);
	init_idt_entry(62, &_isr62);
	init_idt_entry(63, &_isr63);
	init_idt_entry(64, &_isr64);
	init_idt_entry(65, &_isr65);
	init_idt_entry(66, &_isr66);
	init_idt_entry(67, &_isr67);
	init_idt_entry(68, &_isr68);
	init_idt_entry(69, &_isr69);
	init_idt_entry(70, &_isr70);
	init_idt_entry(71, &_isr71);
	init_idt_entry(72, &_isr72);
	init_idt_entry(73, &_isr73);
	init_idt_entry(74, &_isr74);
	init_idt_entry(75, &_isr75);
	init_idt_entry(76, &_isr76);
	init_idt_entry(77, &_isr77);
	init_idt_entry(78, &_isr78);
	init_idt_entry(79, &_isr79);
	init_idt_entry(80, &_isr80);
	init_idt_entry(81, &_isr81);
	init_idt_entry(82, &_isr82);
	init_idt_entry(83, &_isr83);
	init_idt_entry(84, &_isr84);
	init_idt_entry(85, &_isr85);
	init_idt_entry(86, &_isr86);
	init_idt_entry(87, &_isr87);
	init_idt_entry(88, &_isr88);
	init_idt_entry(89, &_isr89);
	init_idt_entry(90, &_isr90);
	init_idt_entry(91, &_isr91);
	init_idt_entry(92, &_isr92);
	init_idt_entry(93, &_isr93);
	init_idt_entry(94, &_isr94);
	init_idt_entry(95, &_isr95);
	init_idt_entry(96, &_isr96);
	init_idt_entry(97, &_isr97);
	init_idt_entry(98, &_isr98);
	init_idt_entry(99, &_isr99);
	init_idt_entry(100, &_isr100);
	init_idt_entry(101, &_isr101);
	init_idt_entry(102, &_isr102);
	init_idt_entry(103, &_isr103);
	init_idt_entry(104, &_isr104);
	init_idt_entry(105, &_isr105);
	init_idt_entry(106, &_isr106);
	init_idt_entry(107, &_isr107);
	init_idt_entry(108, &_isr108);
	init_idt_entry(109, &_isr109);
	init_idt_entry(110, &_isr110);
	init_idt_entry(111, &_isr111);
	init_idt_entry(112, &_isr112);
	init_idt_entry(113, &_isr113);
	init_idt_entry(114, &_isr114);
	init_idt_entry(115, &_isr115);
	init_idt_entry(116, &_isr116);
	init_idt_entry(117, &_isr117);
	init_idt_entry(118, &_isr118);
	init_idt_entry(119, &_isr119);
	init_idt_entry(120, &_isr120);
	init_idt_entry(121, &_isr121);
	init_idt_entry(122, &_isr122);
	init_idt_entry(123, &_isr123);
	init_idt_entry(124, &_isr124);
	init_idt_entry(125, &_isr125);
	init_idt_entry(126, &_isr126);
	init_idt_entry(127, &_isr127);
	init_idt_entry(128, &_isr128);
	init_idt_entry(129, &_isr129);
	init_idt_entry(130, &_isr130);
	init_idt_entry(131, &_isr131);
	init_idt_entry(132, &_isr132);
	init_idt_entry(133, &_isr133);
	init_idt_entry(134, &_isr134);
	init_idt_entry(135, &_isr135);
	init_idt_entry(136, &_isr136);
	init_idt_entry(137, &_isr137);
	init_idt_entry(138, &_isr138);
	init_idt_entry(139, &_isr139);
	init_idt_entry(140, &_isr140);
	init_idt_entry(141, &_isr141);
	init_idt_entry(142, &_isr142);
	init_idt_entry(143, &_isr143);
	init_idt_entry(144, &_isr144);
	init_idt_entry(145, &_isr145);
	init_idt_entry(146, &_isr146);
	init_idt_entry(147, &_isr147);
	init_idt_entry(148, &_isr148);
	init_idt_entry(149, &_isr149);
	init_idt_entry(150, &_isr150);
	init_idt_entry(151, &_isr151);
	init_idt_entry(152, &_isr152);
	init_idt_entry(153, &_isr153);
	init_idt_entry(154, &_isr154);
	init_idt_entry(155, &_isr155);
	init_idt_entry(156, &_isr156);
	init_idt_entry(157, &_isr157);
	init_idt_entry(158, &_isr158);
	init_idt_entry(159, &_isr159);
	init_idt_entry(160, &_isr160);
	init_idt_entry(161, &_isr161);
	init_idt_entry(162, &_isr162);
	init_idt_entry(163, &_isr163);
	init_idt_entry(164, &_isr164);
	init_idt_entry(165, &_isr165);
	init_idt_entry(166, &_isr166);
	init_idt_entry(167, &_isr167);
	init_idt_entry(168, &_isr168);
	init_idt_entry(169, &_isr169);
	init_idt_entry(170, &_isr170);
	init_idt_entry(171, &_isr171);
	init_idt_entry(172, &_isr172);
	init_idt_entry(173, &_isr173);
	init_idt_entry(174, &_isr174);
	init_idt_entry(175, &_isr175);
	init_idt_entry(176, &_isr176);
	init_idt_entry(177, &_isr177);
	init_idt_entry(178, &_isr178);
	init_idt_entry(179, &_isr179);
	init_idt_entry(180, &_isr180);
	init_idt_entry(181, &_isr181);
	init_idt_entry(182, &_isr182);
	init_idt_entry(183, &_isr183);
	init_idt_entry(184, &_isr184);
	init_idt_entry(185, &_isr185);
	init_idt_entry(186, &_isr186);
	init_idt_entry(187, &_isr187);
	init_idt_entry(188, &_isr188);
	init_idt_entry(189, &_isr189);
	init_idt_entry(190, &_isr190);
	init_idt_entry(191, &_isr191);
	init_idt_entry(192, &_isr192);
	init_idt_entry(193, &_isr193);
	init_idt_entry(194, &_isr194);
	init_idt_entry(195, &_isr195);
	init_idt_entry(196, &_isr196);
	init_idt_entry(197, &_isr197);
	init_idt_entry(198, &_isr198);
	init_idt_entry(199, &_isr199);
	init_idt_entry(200, &_isr200);
	init_idt_entry(201, &_isr201);
	init_idt_entry(202, &_isr202);
	init_idt_entry(203, &_isr203);
	init_idt_entry(204, &_isr204);
	init_idt_entry(205, &_isr205);
	init_idt_entry(206, &_isr206);
	init_idt_entry(207, &_isr207);
	init_idt_entry(208, &_isr208);
	init_idt_entry(209, &_isr209);
	init_idt_entry(210, &_isr210);
	init_idt_entry(211, &_isr211);
	init_idt_entry(212, &_isr212);
	init_idt_entry(213, &_isr213);
	init_idt_entry(214, &_isr214);
	init_idt_entry(215, &_isr215);
	init_idt_entry(216, &_isr216);
	init_idt_entry(217, &_isr217);
	init_idt_entry(218, &_isr218);
	init_idt_entry(219, &_isr219);
	init_idt_entry(220, &_isr220);
	init_idt_entry(221, &_isr221);
	init_idt_entry(222, &_isr222);
	init_idt_entry(223, &_isr223);
	init_idt_entry(224, &_isr224);
	init_idt_entry(225, &_isr225);
	init_idt_entry(226, &_isr226);
	init_idt_entry(227, &_isr227);
	init_idt_entry(228, &_isr228);
	init_idt_entry(229, &_isr229);
	init_idt_entry(230, &_isr230);
	init_idt_entry(231, &_isr231);
	init_idt_entry(232, &_isr232);
	init_idt_entry(233, &_isr233);
	init_idt_entry(234, &_isr234);
	init_idt_entry(235, &_isr235);
	init_idt_entry(236, &_isr236);
	init_idt_entry(237, &_isr237);
	init_idt_entry(238, &_isr238);
	init_idt_entry(239, &_isr239);
	init_idt_entry(240, &_isr240);
	init_idt_entry(241, &_isr241);
	init_idt_entry(242, &_isr242);
	init_idt_entry(243, &_isr243);
	init_idt_entry(244, &_isr244);
	init_idt_entry(245, &_isr245);
	init_idt_entry(246, &_isr246);
	init_idt_entry(247, &_isr247);
	init_idt_entry(248, &_isr248);
	init_idt_entry(249, &_isr249);
	init_idt_entry(250, &_isr250);
	init_idt_entry(251, &_isr251);
	init_idt_entry(252, &_isr252);
	init_idt_entry(253, &_isr253);
	init_idt_entry(254, &_isr254);
	init_idt_entry(255, &_isr255);

	set_pit_freq(TIMER_FREQ);

    // Charger l'IDT dans le registre IDTR
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base = (u32) &idt;
    asm volatile("lidt %0" : : "m" (idt_ptr));
	asm volatile ("sti");
}
