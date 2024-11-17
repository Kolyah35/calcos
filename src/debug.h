#include <uart.h>
#include <platform.h>

#ifdef DEBUG
    #ifdef PLATFORM_AVR
        #define print_func uart_print
    #else 
        #define print_func printf
    #endif

    #define dbg_info(fmt, ...) print_func("INFO %s:%d: %s", __FILE__, __LINE__, fmt, ...)
    #define dbg_warn(fmt, ...) print_func("WARN %s:%d: %s", __FILE__, __LINE__, fmt, ...)
    #define dbg_err(fmt, ...) print_func("ERR %s:%d: %s", __FILE__, __LINE__, fmt, ...)
#else
    #define dbg_info(fmt, ...)
    #define dbg_warn(fmt, ...)
    #define dbg_err(fmt, ...) 
#endif