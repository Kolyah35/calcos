#pragma once

#ifdef _AVR_ARCH_
    #define CALCOS_AVR
#else
    #define PROGMEM
    #define CALCOS_SIM
#endif