#include <hal/memory.h>

#undef __INT24_MAX__
#include <avr/pgmspace.h>

void hal_memory_read_module(module_t* dest, module_t* src) {
    memcpy_PF(dest, src, sizeof(module_t));
}