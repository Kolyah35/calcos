#pragma once
#include <stddef.h>

extern size_t allocated_mem = 0;
extern size_t total_mem = 2048; // temp value

void* mem_malloc(size_t size);
void* mem_calloc(size_t nmemb, size_t size);
void* mem_realloc(void* ptr, size_t size);
void mem_free(void* ptr);