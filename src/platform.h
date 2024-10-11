#pragma once

#ifdef PLATFORM_SIM
    #include <string.h>
    
    #define PROGMEM
    #define memcpy_P memcpy
    #define memcpy_PF memcpy
#endif