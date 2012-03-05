#include <minictk.h>
#include <string.h>
#include <stdio.h>

static int compare(void *s0, void *s1)
{
	return strcmp(s0, s1);
}

static void callback(void *key, void *data, void *param)
{
	printf("Callback with item [%s] and data [%s].\n",
			(const char*) key,
			(const char*) data);
}

int main(int argc, char **argv)
{
	ptree_t pt;
	ptree_create(&pt, 20, compare);

	ptree_pack(&pt, "Data 3", 0, "Key 3", 0);
	ptree_pack(&pt, "Data 2", 0, "Key 2", 0);
	ptree_pack(&pt, "Data 4", 0, "Key 4", 0);
	ptree_pack(&pt, "Data 1", 0, "Key 1", 0);

	ptree_iterate(&pt, callback, "Hello");
	printf("The data of key 3 is [%s]\n", (const char*) ptree_fetch(&pt, "Key 3"));

	ptree_destroy(&pt);
	return 0;
}

