#ifndef MODULES_H
#define MODULES_H
#include <stdint.h>

typedef void (*obs_func_t) (void*);

typedef struct {
	obs_func_t func;
} obs_bind_t;

typedef struct {
	uint16_t count, max;
	obs_bind_t list[0];
} observer_t;
#define OBSERVER_PAD(max) union{observer_t s;char p[sizeof(observer_t)+sizeof(obs_bind_t)*max];}
#define OBSERVER_PTR(obs) ((observer_t*) obs)

void obs_reset(observer_t *o, uint16_t max);
void obs_register(observer_t *o, obs_func_t func);
void obs_unregister(observer_t *o, obs_func_t func);
void obs_trigger(observer_t *o, void *data);

#endif

