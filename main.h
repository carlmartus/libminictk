#ifndef MAIN_H
#define MAIN_H
#include <stdint.h>

enum {
	MCTK_SIG_NONE=0,
	MCTK_SIG_MEMORY_LEAK,
	MCTK_SIG_MEMORY_FULL,
	MCTK_SIG_OBSERVER_FULL,
};

typedef union {
	struct {
		uint32_t amount;
		uint32_t size;
	} leak;
} mctk_sigdata_t;

typedef void(*minictk_sig_t)(int, mctk_sigdata_t);

void minictk_start();
void minictk_stop();
void minictk_signal(minictk_sig_t sig);
void minictk_signal_call(int code, mctk_sigdata_t data);
const char *minictk_version();

#endif

