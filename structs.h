#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdint.h>

typedef struct {
	uint32_t size, alloc;
	void *data;
} dbuf_t;

void dbuf_create(dbuf_t *db, uint32_t init_size);
void dbuf_destroy(dbuf_t *db);
void *dbuf_pack(dbuf_t *db, void *ptr, uint32_t size);
void *dbuf_reserve(dbuf_t *db, uint32_t size);

#endif

