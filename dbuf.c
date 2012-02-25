#include "structs.h"
#include "mem.h"
#include <string.h>

#define ZERO_START 64

void dbuf_create(dbuf_t *db, uint32_t init_size)
{
	db->size = 0;
	db->alloc = init_size;
	db->data = mem_malloc(db->alloc);
}

void dbuf_destroy(dbuf_t *db)
{
	db->alloc = 0;
	db->size = 0;
	mem_free(db->data);
}

static void reserve(dbuf_t *db, uint32_t new_size)
{
	uint32_t grow;
	if (new_size < db->alloc) {
		return;
	}

	grow = db->alloc;
	if (grow <= 0) {
		grow = ZERO_START;
	}
	while (grow < new_size) {
		grow <<= 1;
	}
	db->data = mem_realloc(db->data, grow);
	db->alloc = grow;
}

void *dbuf_pack(dbuf_t *db, void *ptr, uint32_t size)
{
	void *dst;
	reserve(db, db->size + size);
	dst = db->data + db->size;
	memcpy(dst, ptr, size);
	db->size += size;
	return dst;
}

void *dbuf_reserve(dbuf_t *db, uint32_t size)
{
	void *ptr = db->data + db->size;
	reserve(db, db->size + size);
	return ptr;
}

