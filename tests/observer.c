#include "epicfail.h"
#include <minictk.h>
#include <stdio.h>
#include <assert.h>

static unsigned counter;
static unsigned sig_count[MCTK_SIG_COUNT];

static void cb_count(void *ptr)
{
	counter++;
}

static void cb_empty(void *ptr) {}

static void cb_sig(int sig, mctk_sigdata_t data)
{
	sig_count[sig]++;
}

void start()
{
	int i;
	minictk_start();
	for (i=0; i<MCTK_SIG_COUNT; i++) {
		sig_count[i] = 0;
	}
	minictk_signal(cb_sig);
}

void stop()
{
	minictk_stop();
}

static int test_startstop()
{
	OBSERVER_PAD(1) opad;
	observer_t *obs = OBSERVER_PTR(&opad);
	obs_reset(obs, 1);
	obs_register(obs, cb_count);
	return 0;
}

static int test_trigger()
{
	OBSERVER_PAD(1) opad;
	observer_t *obs = OBSERVER_PTR(&opad);
	obs_reset(obs, 1);
	obs_register(obs, cb_count);

	counter = 0;
	obs_trigger(obs, 0);
	obs_trigger(obs, 0);
	assert(counter == 2);
	return 0;
}

static int test_overflow()
{
	int i;
	OBSERVER_PAD(8) opad;
	observer_t *obs = OBSERVER_PTR(&opad);
	counter = 0;
	obs_reset(obs, 8);

	for (i=0; i<12; i++) {
		obs_register(obs, cb_empty);
	}
	assert(sig_count[MCTK_SIG_OBSERVER_FULL] == 4);
	return 0;
}

epicfail_case_t manifest[] = {
	{ "Stack allocation", test_startstop },
	{ "Trigger", test_trigger },
	{ "Overflow", test_overflow },
	{ 0, 0 },
};
