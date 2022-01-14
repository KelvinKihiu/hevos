#include "timer.h"
#include "kernel.h"

uint32_t pit_ticks = 0;

void timer_tick()
{
    pit_ticks++;
}


void timer_wait(uint32_t ms)
{
    uint32_t now = pit_ticks;
    ++ms;
    while ((pit_ticks - now) < ms) { ; }
}