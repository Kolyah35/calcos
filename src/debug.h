#include <uart.h>
#include <platform.h>

#ifdef DEBUG
    #ifdef PLATFORM_AVR
        #define print_func uart_print
    #else 
        #include <stdio.h>
        #define print_func printf
    #endif

    #define dbg_info(fmt, ...) print_func("INFO %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
    #define dbg_warn(fmt, ...) print_func("WARN %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
    #define dbg_err(fmt, ...) print_func("ERR %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#else
    #define dbg_info(fmt, ...)
    #define dbg_warn(fmt, ...)
    #define dbg_err(fmt, ...) 
#endif