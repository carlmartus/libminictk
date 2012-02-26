#include "modules.h"
#include "main.h"

void obs_reset(observer_t *o, uint16_t max)
{
	o->count = 0;
	o->max = max;
}

static void full(observer_t *o)
{
	mctk_sigdata_t sig;
	sig.observer_ptr = o;
	minictk_signal_call(MCTK_SIG_OBSERVER_FULL, sig);
}

void obs_register(observer_t *o, obs_func_t func)
{
	obs_bind_t bind;
	if (o->count >= o->max) {
		full(o);
		return;
	}

	bind.func = func;
	o->list[o->count] = bind;
	o->count++;
}

void obs_unregister(observer_t *o, obs_func_t func)
{
	int i;
	for (i=0; i<o->count; i++) {
		if (o->list[i].func == func) {
			break;
		}
	}

	if (i < o->count) {
		for (; i<o->count-1; i++) {
			o->list[i] = o->list[i+1];
		}
		o->count--;
	}
}

void obs_trigger(observer_t *o, void *data)
{
	int i;
	for (i=0; i<o->count; i++) {
		o->list[i].func(data);
	}
}

