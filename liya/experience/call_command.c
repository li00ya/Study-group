#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

static int32_t test_call_cmd(void* cmd, void* path, void* filename, uint8_t space)
{
	int8_t command[256] = {0};
	int8_t *ptr = command;
	uint32_t len;

	if (NULL == cmd) {
		return -1;
	}

	len = strlen(cmd);
	memcpy(ptr, cmd, len);
	ptr += len;

	if (' ' != *(ptr - 1)) {
		*ptr++ = ' ';
	}

	if (NULL != path) {
		len = strlen(path);
		memcpy(ptr, path, len);
		ptr += len;

		if ('/' != *(ptr - 1)) {
			*ptr++ = '/';
		}
	}

	if (1 == space) {
		*ptr++ = ' ';
	}

	if (NULL != filename) {
		len = strlen(filename);
		memcpy(ptr, filename, len);
	}

	system(command);

	return 0;
}

static void test_usage(int8_t* tip)
{
	printf("Usage: %s <param>\n"
			"\nparam:\n"
			"\tmkdir\t[path]	create a folder.\n"
			"\trmdir\t[path]	remove the folder of create.\n"
			"\tpath		if not set, default path = test/\n", tip);
}

int32_t main(int32_t ac, int8_t** av)
{
	int8_t *dir = "mkdir -p";
	int8_t *rmdir = "rm -r";
	int8_t path[256] = "test/";

	if (ac < 2) {
		test_usage(av[0]);
		return 0;
	}

	ac -= 2;
	if (ac > 0) {
		memset(path, 0, sizeof(path));
		memcpy(path, av[2], strlen(av[2]));
	}

	switch (*av[1]) {
	case 'm':
		test_call_cmd(dir, path, NULL, 0);
		test_call_cmd("ls -l", NULL, NULL, 0);
		break;
	case 'r':
		test_call_cmd(rmdir, path, NULL, 0);
		test_call_cmd("ls -l", NULL, NULL, 0);
		break;
	default:
		test_usage(av[0]);
		break;
	}

	return 0;
}
