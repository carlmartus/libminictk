#include "mem.h"
#include "main.h"
#include <stdlib.h>
#include <stdint.h>

typedef struct {
	uint32_t size;
} prefix_t;

static uint32_t active, sumsize;

void mem_start()
{
	active = 0;
	sumsize = 0;
}

void mem_stop()
{
	mctk_sigdata_t info;

	if (active > 0) {
		info.leak.amount = active;
		info.leak.size = sumsize;
		minictk_signal_call(MCTK_SIG_MEMORY_LEAK, info);
	}
}


void *mem_malloc(uint32_t size)
{
	void *ptr;
	active++;
	sumsize += size;

	size += sizeof(prefix_t);
	ptr = malloc(size);
	((prefix_t*) ptr)->size = size;
	ptr += sizeof(prefix_t);
	return ptr;
}

void *mem_realloc(void *ptr, uint32_t new_size)
{
	new_size += sizeof(prefix_t);
	ptr = realloc(ptr-sizeof(prefix_t), new_size);
	((prefix_t*) ptr)->size = new_size;
	return ptr + sizeof(prefix_t);
}

uint32_t mem_size(void *ptr)
{
	ptr -= sizeof(prefix_t);
	return ((prefix_t*) ptr)->size;
}

uint32_t mem_free(void *ptr)
{
	uint32_t size;
	ptr -= sizeof(prefix_t);
	size = ((prefix_t*) ptr)->size;
	free(ptr);
	return size;
}

