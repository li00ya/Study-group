#include "api/mode.h"
#include "util/string.h"
#include "util/memory.h"
#include "util/types.h"
#include "util/misc.h"
#include "util/file.h"

static int32_t test_mkdir_path(void* path)
{
	int8_t cmd[256] = {0};
	int8_t tmp[128] = {0};

	if (NULL == path) {
		util_strcpy(tmp, "test/");
	} else {
		util_strcpy(tmp, path);
	}

	util_snprintf(cmd, sizeof(cmd), "mkdir -p %s", tmp);
	util_puts("Create %s folder\n", (int8_t *)tmp);
	util_exec(cmd);

	return 0;
}

static int32_t test_rm_data(void* data)
{
	int8_t cmd[256] = {0};

	if (NULL == data) {
		util_puts("param error\n");
		return -1;
	}

	util_snprintf(cmd, sizeof(cmd), "rm -rf %s", data);
	util_exec(cmd);

	return 0;

}

static int32_t test_zip_package(void* filename, void* path)
{
	int8_t cmd[256] = {0};

	if (NULL == filename || NULL== path) {
		util_puts("param error\n");
		return -1;
	}

	if (util_file_access(path, FILE_FLAG_EXIST)) {
		test_mkdir_path(path);
	}

	util_snprintf(cmd, sizeof(cmd), "zip -r %s %s", filename, path);
	util_exec(cmd);

	return 0;
}

static int32_t test_unzip_package(void* filename, void* path)
{
	int8_t cmd[256] = {0};

	if (NULL == filename || NULL== path) {
		util_puts("param error\n");
		return -1;
	}

	if (util_file_access(path, FILE_FLAG_EXIST)) {
		test_mkdir_path(path);
	}

	util_snprintf(cmd, sizeof(cmd), "unzip -q -d %s %s", path, filename);
	util_exec(cmd);

	return 0;
}

static int32_t test_usage(void)
{
	util_puts("system cmd option:\n"
			"\tmkdir\t[path]\n"
			"\trm\t[data]\n"
			"\tzip\t[path] [zip name]\n"
			"\tunzip\t[path] [zip name]\n");

	return 0;
}

static int32_t sys_proc(int32_t ac, int8_t* av[])
{
	switch (*av[0]) {
	case 'm':
		test_mkdir_path(av[1]);
		break;
	case 'r':
		test_rm_data(av[1]);
		break;
	case 'z':
		test_zip_package(av[2], av[1]);
		break;
	case 'u':
		test_unzip_package(av[2], av[1]);
		break;
	default:
		test_usage();
		break;
	}

	return 0;
}

static lee_module_t sys_module = {
	.name = "system",
	.usage = test_usage,
	.proc = sys_proc,
};

static void PREP_INIT sys_constructor(void)
{
	lee_module_register(LEE_TYPE_EXP, LEE_EXP_SYS, &sys_module);
}

static void PREP_EXIT sys_destructor(void)
{
	lee_module_unregister(LEE_TYPE_EXP, LEE_EXP_SYS);
}

