#include <mem.h>
#include <stdint.h>

extern uint8_t __data_start;
extern uint8_t __data_end;
extern uint8_t __bss_start;
extern uint8_t __bss_end;
extern uint8_t __heap_start;
extern uint8_t *__brkval;

unsigned int get_used_mem() {
    uint8_t *heap_end = __brkval ? __brkval : &__heap_start;
    return (unsigned int)(heap_end - &__data_start);
}