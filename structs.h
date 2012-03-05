#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdint.h>

// Dynamic Buffer

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
inline void *dbuf_data(dbuf_t *db);
inline uint32_t dbuf_offset(dbuf_t *db, void *ptr);
inline void *dbuf_get(dbuf_t *db, uint32_t offset);
dbuf_ptr_t dbuf_get_definite(dbuf_t *db, void *ptr);
void *dbuf_ptr_retrive(dbuf_ptr_t dp);


// Packed Tree

typedef int32_t (*pt_compare_t)(void*, void*);

typedef struct {
	uint32_t key, data;
	uint32_t lt, gt;
} pt_node_t;

#define PTREE_ROOT 1

typedef struct {
	uint8_t flags;
	dbuf_t db;
	pt_compare_t cmp;
	uint32_t root;
} ptree_t;

typedef void (*pt_callback_t) (void *key, void *data, void *param);

void ptree_create(ptree_t *pt, uint32_t init_size, pt_compare_t cmp);
void ptree_destroy(ptree_t *pt);
void ptree_pack(ptree_t *pt, void *data, uint32_t dsize, void *key, uint32_t ksize);
void *ptree_fetch(ptree_t *pt, void *key);
void ptree_iterate(ptree_t *ptr, pt_callback_t cb, void *param);

#endif

