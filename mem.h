#ifndef MEM_H
#define MEM_H
#include <stdint.h>

void mem_start();
void mem_stop();
void *mem_malloc(uint32_t size);
void *mem_realloc(void *ptr, uint32_t new_size);
uint32_t mem_size(void *ptr);
uint32_t mem_free(void *ptr);

#endif
