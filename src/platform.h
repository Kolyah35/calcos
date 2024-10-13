#pragma once


#ifdef PLATFORM_AVR
    #include <io/pgm_space.h>
#endif

#ifdef PLATFORM_SIM
    #include <string.h>
    
    #define PROGMEM
    #define memcpy_P memcpy
    #define memcpy_PF memcpy

    #ifdef _WIN32
        // fuck i can't compile it
        // #define MMNOSOUND
        // #define NODRAWTEXT
        // #define NOUSER
        // #define NOGDI
        // #define INC_OLE1
        // #include <Windows.h>
        // #undef far
        // #undef near
    #endif
#endif