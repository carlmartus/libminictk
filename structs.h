/**
 * @file
 * @breif Data structures
 * Contains a few data structure classes
 */


#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdint.h>

// ===============
//  Dynamic Buffer
// ===============
typedef struct {
	uint32_t size;
	uint32_t alloc;
} dbuf_t;

/**
 * Initialize structure for usage.
 * @param db Instance
 * @param init_size Initial size to be allocated
 */
void dbuf_create(dbuf_t *db, uint32_t init_size);

/**
 * Clear dynamic buffer
 * @param db Instance
 */
void dbuf_destroy(dbuf_t *db);


#endif

