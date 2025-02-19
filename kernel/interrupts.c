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


typedef struct {
	register_t regs;
	u8 exists;
} process_t;

int current_process = 0;
process_t processes[10] = {0};
int process_count = 0;

#define PROCESS_STACK_SIZE      1500000 // 1.5MB
#define PROCESS_STACK_START 500000000 // 500MB
#define PRELINKED_PROG_ADDR 1000000000 // 1GB


int new_process(void *entry_point) {
	if (process_count >= 10)
		return -1;
	int process_id = process_count;
	u32 stack = PROCESS_STACK_START + PROCESS_STACK_SIZE * (process_count + 1);
	processes[process_id].regs = (register_t){0};
	processes[process_id].regs.esp = stack;
	processes[process_id].regs.ebp = stack;
	processes[process_id].regs.eip = (u32)entry_point;
	processes[process_id].regs.cs = 0x08;
	processes[process_id].regs.ds = 0x10;
	processes[process_id].regs.eflags = 0x202; // IF=1 pour activer les interruptions
	processes[process_id].exists = 1;
	process_count++;
	return process_id;
}

void dump_processes() {
	fprintf(serialout, "Processes ccount: %d\n", process_count);
	for (int i = 0; i < process_count; i++) {
		fprintf(serialout, "Process %d: %d %x %x\n", i, processes[i].exists, processes[i].regs.eip);
	}
}


void irq_handler_c(register_t *r) {
	if (r->int_no >= 40)
        port_write_u8(0xA0, 0x20);
    port_write_u8(0x20, 0x20);
	if (r->int_no == 32) {
		// timer
		dump_processes();
		if (process_count > 0) {
			// save current
			processes[current_process].regs = *r;

			// switch
			current_process++;
			if (current_process >= process_count)
				current_process = 0;
			if (processes[current_process].exists) {
				*r = processes[current_process].regs;
			}
		}
	}
}

void isr_handler_c(register_t *r) {
	if ((i32)r->int_no < 0)
		r->int_no += 256;
	fprintf(stderr, "ISR %d\n", r->int_no);
}