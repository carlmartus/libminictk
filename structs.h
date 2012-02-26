#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdint.h>

typedef struct {
	uint32_t size, alloc;
	void *data;
} dbuf_t;

typedef struct {
	dbuf_t *db;
	uint32_t offset;
} dbuf_ptr_t;

void dbuf_create(dbuf_t *db, uint32_t init_size);
void dbuf_destroy(dbuf_t *db);
void *dbuf_pack(dbuf_t *db, void *ptr, uint32_t size);
void *dbuf_reserve(dbuf_t *db, uint32_t size);
inline uint32_t dbuf_offset(dbuf_t *db, void *ptr);
inline void *dbuf_retrive(dbuf_t *db, uint32_t offset);
dbuf_ptr_t dbuf_get_definite(dbuf_t *db, void *ptr);
void *dbuf_ptr_retrive(dbuf_ptr_t dp);

#endif

