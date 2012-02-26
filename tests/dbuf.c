#include "epicfail.h"
#include <minictk.h>
#include <assert.h>

void start()
{
	minictk_start();
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
	dbuf_create(&db, 500);

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

epicfail_case_t manifest[] = {
	{ "Create and destroy", test_startstop },
	{ "Buffer stair growth", test_stairs },
	{ 0, 0 },
};
