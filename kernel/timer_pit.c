#include <kernel.h>

void set_pit_freq(u32 freq) {
	if (freq == 0) return;
	u32 divisor = 1193180 / freq;
	if (divisor > 65535) divisor = 65535;
	if (divisor < 1) divisor = 1;

	port_write_u8(0x43, 0x36);
	port_write_u8(0x40, divisor & 0xFF);
	port_write_u8(0x40, divisor >> 8);
}

u32 timer_get_ms() {
	extern u32 _timer_time_ms;
	return _timer_time_ms;
}

void sleep_ms(u32 ms) {
	u32 end = timer_get_ms() + ms;
	while (timer_get_ms() < end)
		asm volatile("hlt");
}