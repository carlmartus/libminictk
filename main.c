#include "main.h"
#include "mem.h"
#include "config.h"
#include <stdio.h>

static minictk_sig_t sigfunc;

void minictk_start()
{
	sigfunc = NULL;
	mem_start();
}

void minictk_stop()
{
	mem_stop();
}

void minictk_signal(minictk_sig_t sig)
{
	sigfunc = sig;
}

void minictk_signal_call(int code, mctk_sigdata_t data)
{
	if (sigfunc != NULL) {
		sigfunc(code, data);
	}
}

const char *minictk_version()
{
	return VERSION;
}

