#include <kernel.h>

u32 factor;

void ltds_timer_init(void) {
    tm_t time;
    time_get(&time);

    int sec = time.tm_sec;
    while (sec == time.tm_sec) {
        time_get(&time);
    }

    factor = 0;

    sec = time.tm_sec;

    while (sec == time.tm_sec) {
        factor++;
        for (int i = 0; i < 10000; i++)
            asm volatile("nop");
        time_get(&time);
    }
}

u32 ltds_timer_sleep(u32 ms) {
    tm_t time;

    u32 start = factor;
    for (int i = 0; i < (ms * factor) / 1000; i++) {
        for (int i = 0; i < 10000; i++)
            asm volatile("nop");
        time_get(&time);
    }
}
