#include "epicfail.h"
#include <minictk.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef struct {
	uint64_t msg;
} data_t;

static void cb_sig(int code, mctk_sigdata_t data)
{
	assert(code == MCTK_SIG_NONE);
}

void start()
{
	minictk_start();
	minictk_signal(cb_sig);
}

void stop()
{
	minictk_stop();
}

static int test_create()
{
	int num[5];
	ptree_t pt;
	ptree_create(&pt, 12, NULL);

	ptree_pack(&pt, &num, sizeof(num), "A", 0);
	ptree_pack(&pt, &num, sizeof(num), "B", 0);
	ptree_pack(&pt, &num, sizeof(num), "C", 0);
	ptree_destroy(&pt);
	return 0;
}

static int test_empty()
{
	void *ptr;
	ptree_t pt;
	ptree_create(&pt, 500, NULL);
	ptr = ptree_fetch(&pt, "empty");
	assert(ptr == NULL);
	ptree_destroy(&pt);
	return 0;
}

static int test_grow()
{
	ptree_t pt;
	uint64_t num;
	int i;
	char msg[40];

	num = 213412434;
	ptree_create(&pt, 1, NULL);

	for (i=0; i<400; i++) {
		sprintf(msg, "Sample key %d", i+1);
		num <<= 1;
		ptree_pack(&pt, &num, sizeof(num), (void*) msg, 0);
	}

	ptree_destroy(&pt);
	return 0;
}

static int test_fetch()
{
	int num;
	ptree_t pt;
	ptree_create(&pt, 400, NULL);

	num = 0;
	ptree_pack(&pt, &num, sizeof(num), "A", 0); num++;
	ptree_pack(&pt, &num, sizeof(num), "B", 0); num++;
	ptree_pack(&pt, &num, sizeof(num), "C", 0);

	assert(0 == *(int*) ptree_fetch(&pt, "A"));
	assert(1 == *(int*) ptree_fetch(&pt, "B"));
	assert(2 == *(int*) ptree_fetch(&pt, "C"));

	ptree_destroy(&pt);
	return 0;
}




static int itr_counter;
static void itr_cb(void *key, void *data, void *param)
{
	const char *str;
	char msg[40];
	int num;

	str = (const char*) key;
	num = *((int*) data);

	assert(*((int*) param) == 42);
	assert(num == itr_counter);

	sprintf(msg, "Message %.2d", itr_counter);
	assert(strcmp(str, msg) == 0);

	itr_counter++;
}

static int test_iterate()
{
	int i;
	char msg[40];
	ptree_t pt;

	ptree_create(&pt, 1, NULL);

	for (i=39; i>=0; i--) {
		sprintf(msg, "Message %.2d", i);

		ptree_pack(&pt, &i, sizeof(i), msg, 0);
	}

	i = 42;
	itr_counter = 0;
	ptree_iterate(&pt, itr_cb, &i);
	assert(itr_counter == 40);
	ptree_destroy(&pt);
	return 0;
}

epicfail_case_t manifest[] = {
	{ "Simple creation", test_create },
	{ "Empty", test_empty },
	{ "Growth", test_grow },
	{ "Iterate", test_iterate },
	{ "Fetch", test_fetch },
	{ 0, 0 },
};
