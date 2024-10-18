#include <memory.h>
#include <stdlib.h>

void* mem_malloc(size_t size) {
    allocated_mem += size;
    return malloc(size);
}

void* mem_calloc(size_t count, size_t size) {
    allocated_mem += count * size;
    return calloc(count, size);
}

void* mem_realloc(void* ptr, size_t size) {
    size_t allocated = *(size_t*)(&ptr - sizeof(size_t) * 2);
    allocated_mem -= allocated + size;
    return realloc(ptr, size);
}

void mem_free(void* ptr) {
    size_t allocated = *(size_t*)(&ptr - sizeof(size_t) * 2);
    allocated_mem -= allocated;
    return free(ptr);
}