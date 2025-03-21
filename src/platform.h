#pragma once

#ifdef PLATFORM_AVR
    #include <avr/pgmspace.h>
#endif

#ifdef PLATFORM_SIM
    #include <string.h>
    #include <stdint.h>

    #define memchr_P memchr
    #define memcmp_P memcmp
    #define memcpy_P memcpy
    #define strcasecmp_P strcasecmp
    #define strcat_P strcat
    #define strchr_P strchr
    #define strcpy_P strcpy
    #define strcspn_P strcspn
    #define strlen_P strlen
    #define strncasecmp_P strncasecmp
    #define strncat_P strncat
    #define strncmp_P strncmp
    #define strncpy_P strncpy
    #define strnlen_P strnlen

    typedef void prog_void;
    typedef char prog_char;
    typedef unsigned char prog_uchar;
    typedef int8_t prog_int8_t;
    typedef uint8_t prog_uint8_t;
    typedef int16_t prog_int16_t;
    typedef uint16_t prog_uint16_t;
    typedef int32_t prog_int32_t;
    typedef uint32_t prog_uint32_t;
    typedef int64_t prog_int64_t;
    typedef uint64_t prog_uint64_t;

    #define PROGMEM
    #define PSTR(s) ((const char *)(s))
    #define pgm_read_byte_near(address) *(uint8_t*)(address)
    #define pgm_read_word_near(address) *(uint16_t*)(*address)
    #define pgm_read_dword_near(address) *(uint32_t*)(address)
    #define pgm_read_byte_far(address) *(uint8_t*)(address)
    #define pgm_read_word_far(address) *(uint16_t*)(address)
    #define pgm_read_dword_far(address) *(uint32_t*)(address)
    #define pgm_read_byte(address) pgm_read_byte_near(address)
    #define pgm_read_word(address) pgm_read_word_near(address)
    #define pgm_read_dword(address) pgm_read_dword_near(address)
    #define PGM_P const prog_char*
    #define PRM_VOID_P const prog_void*

    #ifdef _WIN32
        #define MMNOSOUND
        #define NODRAWTEXT
        #define NOUSER
        #define NOGDI

        typedef struct tagMSG* LPMSG;

        #include <Windows.h>
        #undef far
        #undef near
    #endif
#endif