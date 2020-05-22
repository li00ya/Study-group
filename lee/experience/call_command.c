#include "util/string.h"
#include "util/memory.h"
#include "util/types.h"
#include "util/misc.h"
#include "util/file.h"

static int32_t test_call_cmd(void* cmd, void* path, void* filename, uint8_t space)
{
	int8_t command[256] = {0};
	int8_t *ptr = command;
	uint32_t len;

	if (NULL == cmd) {
		return -1;
	}

	len = util_strlen(cmd);
	util_memcpy(ptr, cmd, len);
	ptr += len;

	if (' ' != *(ptr - 1)) {
		*ptr++ = ' ';
	}

	if (NULL != path) {
		len = util_strlen(path);
		util_memcpy(ptr, path, len);
		ptr += len;

		if ('/' != *(ptr - 1)) {
			*ptr++ = '/';
		}
	}

	if (1 == space) {
		*ptr++ = ' ';
	}

	if (NULL != filename) {
		len = util_strlen(filename);
		util_memcpy(ptr, filename, len);
	}

	util_exec(command);

	return 0;
}

static int32_t test_unzip_package(void* filename, void* path)
{
	if (util_file_access(path, FILE_FLAG_EXIST)) {
		util_puts("Create %s folder\n", (int8_t *)path);
		test_call_cmd("mkdir -p", path, NULL, 0);
	}

	test_call_cmd("rm -f", path, "*", 0);
	test_call_cmd("unzip -o -q -d", path, filename, 1);
	test_call_cmd("rm -f", NULL, filename, 0);

	return 0;
}

static void test_usage(int8_t* tip)
{
	util_puts("Usage: %s <param>\n"
			"\nparam:\n"
			"\tmkdir\t[path]\tcreate a folder.\n"
			"\trmdir\t[path]\tremove the folder of create.\n"
			"\tunzip\t[path] [package]\tdecompressed a zip package.\n"
			"\tpackage\t to be decompressed.\n"
			"\tpath\ta folder(if not set, default path = test/)\n", tip);
}

int32_t main(int32_t ac, int8_t* av[])
{
	int8_t *dir = "mkdir -p";
	int8_t *rmdir = "rm -r";
	int8_t *ptr;
	int8_t path[256] = "test/";
	int8_t name[256] = {0};

	if (ac < 2) {
		test_usage(av[0]);
		return 0;
	}

	ptr = util_strrchr(av[1], 'u');
	if (NULL == ptr) {
		if (3 == ac) {
			util_memset(path, 0, sizeof(path));
			util_snprintf(path, sizeof(path), "%s", av[2]);
		}
	} else {
		ptr = util_strrchr(av[2], '.');
		if (NULL != ptr) {
			*ptr = 0;
			util_snprintf(name, sizeof(name), "%s.zip", av[2]);
			*ptr = '.';
		} else {	
			ptr = util_strrchr(av[3], '.');
			if (NULL == ptr) {
				util_puts("input package filename error.\n");
				return -1;
			}
			*ptr = 0;
			util_snprintf(name, sizeof(name), "%s.zip", av[3]);
			*ptr = '.';
			util_memset(path, 0, sizeof(path));
			util_snprintf(path, sizeof(path), "%s", av[2]);
		}

		if (util_file_access(name, 0)) {
			util_puts(" %s not find.\n", name);
			return -1;
		}
	}

	switch (*av[1]) {
	case 'm':
		test_call_cmd(dir, path, NULL, 0);
		break;
	case 'r':
		test_call_cmd(rmdir, path, NULL, 0);
		break;
	case 'u':
		test_unzip_package(name, path);
		break;
	default:
		test_usage(av[0]);
		break;
	}

	test_call_cmd("ls -l", NULL, NULL, 0);

	return 0;
}
