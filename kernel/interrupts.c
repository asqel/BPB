#include <kernel.h>
#include <oeuf.h>

typedef struct {
	u32 ds;
	u32 edi;
	u32 esi;
	u32 ebp;
	u32 esp;
	u32 ebx;
	u32 edx;
	u32 ecx;
	u32 eax;
	u32 int_no;
	u32 error_and_irq;
	u32 eip;
	u32 cs;
	u32 eflags;
	u32 useresp;
	u32 ss;
} register_t;


void irq_handler_c(register_t *r) {
	if (r->int_no >= 40)
        port_write_u8(0xA0, 0x20);
    port_write_u8(0x20, 0x20);
}

void isr_handler_c(register_t *r) {
	if ((i32)r->int_no < 0)
		r->int_no += 256;
	fprintf(stderr, "ISR %d\n", r->int_no);
}