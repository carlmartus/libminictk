#include "epicfail.h"
#include <minictk.h>
#include <assert.h>
#include <string.h>

static void signal_cb(int code, mctk_sigdata_t data)
{
	assert(code == -1);
}

void start()
{
	minictk_start();
	minictk_signal(signal_cb);
}

void stop()
{
	minictk_stop();
}

static int test_startstop()
{
	dbuf_t db;
	dbuf_create(&db, 500);
	dbuf_destroy(&db);
	return 0;
}

static int test_stairs()
{
	int i, allo, put;
	char *itr, *end;

	dbuf_t db;
	dbuf_create(&db, 300);

	allo = 400;
	put = 0;
	for (i=0; i<6; i++) {
		itr = (char*) dbuf_reserve(&db, allo);
		put += allo;
		end = itr + allo;

		while (itr < end) {
			*itr = i & 0xff;
			itr++;
		}
		allo <<= 1;
	}

	assert(db.alloc > put);
	dbuf_destroy(&db);
	return 0;
}

static int test_content()
{
	int i;
	unsigned long int num = 123456, *ptr;
	dbuf_t db;
	dbuf_ptr_t abs;

	dbuf_create(&db, 2);

	ptr = dbuf_reserve(&db, 30);
	abs = dbuf_get_definite(&db, ptr);

	*ptr = num;
	assert(*ptr == num);

	for (i=0; i<5; i++) {
		ptr = dbuf_pack(&db, &num, sizeof(num));
		dbuf_reserve(&db, db.alloc);

		assert(*ptr == num);
	}
	assert(*ptr == num);

	ptr = dbuf_ptr_retrive(abs);
	assert(*ptr == num);

	dbuf_destroy(&db);
	return 0;
}

static int test_reserve()
{
	dbuf_t db;
	int i, cmp;
	char random[500];
	void *ptr;

	dbuf_create(&db, 5);

	for (i=0; i<15; i++) {
		ptr = dbuf_reserve(&db, sizeof(random));
		memcpy(ptr, random, sizeof(random));
	}

	ptr = dbuf_data(&db);
	for (i=0; i<15; i++) {
		cmp = memcmp(ptr, random, sizeof(random));
		assert(cmp == 0);
		ptr += sizeof(random);
	}

	dbuf_destroy(&db);
	return 0;
}

epicfail_case_t manifest[] = {
	{ "Create and destroy", test_startstop },
	{ "Buffer stair growth", test_stairs },
	{ "Packed content", test_content },
	{ "Reserving", test_reserve },
	{ 0, 0 },
};
