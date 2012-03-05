#include "structs.h"
#include <stdio.h>
#include <string.h>

void ptree_create(ptree_t *pt, uint32_t init_size, pt_compare_t cmp)
{
	dbuf_create(&pt->db, init_size);
	pt->cmp = cmp;
	if (pt->cmp == NULL) {
		pt->cmp = (pt_compare_t) strcmp;
	}
	pt->flags = 0;
}

void ptree_destroy(ptree_t *pt)
{
	dbuf_destroy(&pt->db);
}

static void rec_insert(ptree_t *pt, pt_node_t *node, pt_node_t *put)
{
	int32_t cmp;
	void *k1, *k2;
	k1 = dbuf_get(&pt->db, put->key);
	k2 = dbuf_get(&pt->db, node->key);

	cmp = pt->cmp(k1, k2);
	if (cmp >= 0) {
		if (node->gt) {
			node = dbuf_get(&pt->db, node->gt);
			rec_insert(pt, node, put);
		} else {
			node->gt = dbuf_offset(&pt->db, put);
		}

	} else {

		if (node->lt) {
			node = dbuf_get(&pt->db, node->lt);
			rec_insert(pt, node, put);
		} else {
			node->lt = dbuf_offset(&pt->db, put);
		}
	}
}

void ptree_pack(ptree_t *pt, void *data, uint32_t dsize, void *key, uint32_t ksize)
{
	void *ptr;
	uint32_t size;
	pt_node_t *node;

	if (ksize <= 0) {
		ksize = strlen((const char*) key) + 1;
	}

	if (dsize <= 0) {
		dsize = strlen((const char*) data) + 1;
	}

	size = sizeof(pt_node_t) + dsize + ksize;
	ptr = dbuf_reserve(&pt->db, size);
	node = ptr;

	ptr += sizeof(pt_node_t);
	key = memcpy(ptr, key, ksize);

	ptr += ksize;
	data = memcpy(ptr, data, dsize);

	node->lt = 0;
	node->gt = 0;
	node->key = dbuf_offset(&pt->db, key);
	node->data = dbuf_offset(&pt->db, data);

	if ((pt->flags & PTREE_ROOT) == 0) {
		pt->root = dbuf_offset(&pt->db, node);
		pt->flags |= PTREE_ROOT;
	} else {
		ptr = dbuf_get(&pt->db, pt->root);
		rec_insert(pt, ptr, node);
	}
}

static void *fetch_rec(ptree_t *pt, pt_node_t *node, void *key)
{
	int32_t cmp;
	void *ptr;

	ptr = dbuf_get(&pt->db, node->key);
	cmp = pt->cmp(key, ptr);

	if (cmp == 0) {
		return dbuf_get(&pt->db, node->data);

	} else if (cmp > 0) {
		if (node->gt == 0) return NULL;
		ptr = dbuf_get(&pt->db, node->gt);
		return fetch_rec(pt, ptr, key);

	} else {
		if (node->lt == 0) return NULL;
		ptr = dbuf_get(&pt->db, node->lt);
		return fetch_rec(pt, ptr, key);
	}

	return NULL;
}

void *ptree_fetch(ptree_t *pt, void *key)
{
	void *ptr;
	if ((pt->flags & PTREE_ROOT) == 0) {
		return NULL;
	}

	ptr = dbuf_get(&pt->db, pt->root);
	return fetch_rec(pt, ptr, key);
}


static void iterate_rec(ptree_t *pt, pt_node_t *node, pt_callback_t cb, void *param)
{
	void *ptr, *key, *data;

	if (node->lt) {
		ptr = dbuf_get(&pt->db, node->lt);
		iterate_rec(pt, ptr, cb, param);
	}

	key = dbuf_get(&pt->db, node->key);
	data = dbuf_get(&pt->db, node->data);

	cb(key, data, param);

	if (node->gt) {
		ptr = dbuf_get(&pt->db, node->gt);
		iterate_rec(pt, ptr, cb, param);
	}
}

void ptree_iterate(ptree_t *pt, pt_callback_t cb, void *param)
{
	pt_node_t *ptr;
	if ((pt->flags & PTREE_ROOT) == 0) {
		return;
	}

	ptr = dbuf_get(&pt->db, pt->root);
	iterate_rec(pt, ptr, cb, param);
}

