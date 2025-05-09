#include <timer.h>
#include <platform.h>

#ifdef PLATFORM_SIM 

#include <raylib.h>

void timer0_init() {
}

uint32_t millis() {
    return GetTime() * 1000;
}

#endif