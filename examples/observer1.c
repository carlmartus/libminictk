#include <minictk.h>
#include <stdio.h>

static void callback1(void *ptr)
{
	printf("Data for callback 1: \"%s\"\n", (const char*) ptr);
}

static void callback2(void *ptr)
{
	printf("Data for callback 2: \"%s\"\n", (const char*) ptr);
}

int main(int argc, char **argv)
{
	OBSERVER_PAD(2) observer; // Observer with maximum of 3 callbacks
	observer_t *obs_ptr = OBSERVER_PTR(&observer);

	obs_reset(obs_ptr, 2);
	obs_register(obs_ptr, callback1);
	obs_register(obs_ptr, callback2); // Maximum amount of callbacks have now been reached
	obs_trigger(obs_ptr, "Hello Observers!");
	return 0;
}

