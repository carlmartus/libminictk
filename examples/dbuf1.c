#include <minictk.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	dbuf_t db;
	char msg[] = "Hello Dynamic buffer!";
	char *in_buffer;

	dbuf_create(&db, 500); // New buffer with 500 b as initial allocation
	in_buffer = dbuf_pack(&db, msg, strlen(msg)+1); // +1 for the strings trailing 0
	dbuf_destroy(&db); // Free buffer and block of memory

	assert(in_buffer != msg);
	printf("%s\n%s\n", msg, in_buffer);
	return 0;
}

