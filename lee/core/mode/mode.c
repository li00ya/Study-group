#include "api/mode.h"
#include "util/string.h"
#include "util/types.h"


static lee_module_t* lee_modules[LEE_TYPE_MAX][LEE_MODE_MAX];
void lee_module_register(uint32_t type, uint32_t idx, lee_module_t* module)
{
	lee_modules[type][idx] = module;
}

void lee_module_unregister(uint32_t type, uint32_t idx)
{
	lee_modules[type][idx] = NULL;
}

static lee_module_t* lee_get_module(uint32_t type, uint32_t idx)
{
	return lee_modules[type][idx];
}

static int32_t show_usage(int8_t* prog)
{
	int32_t type, idx;
	lee_module_t* module;

	util_puts("usage %s module [option]\n", prog);
	util_puts("module name:\n\t");
	for (type = 0; type < LEE_TYPE_MAX; type++) {
		for (idx = 0; idx < LEE_MODE_MAX; idx++) {
			module = lee_get_module(type, idx);
			if (NULL == module) {
				continue;
			}
			util_puts("%s,", module->name);
		}
	}
	util_puts("\n");

	for (type = 0; type < LEE_TYPE_MAX; type++) {
		for (idx = 0; idx < LEE_MODE_MAX; idx++) {
			module = lee_get_module(type, idx);
			if (NULL == module) {
				continue;
			}

			module->usage();
			util_puts("\n");
		}
	}
	
	return 0;
}

static lee_module_t* lee_find_module(int8_t* name)
{
	lee_module_t* module;
	int32_t type, idx;

	for (type = 0; type < LEE_TYPE_MAX; type++) {
		for (idx = 0; idx < LEE_MODE_MAX; idx++) {
			module = lee_get_module(type, idx);
			if (NULL == module) {
				continue;
			}
		
			if (util_strcmp(name, module->name)) {
				continue;
			}

			return module;
		}
	}

	return NULL;
}

int32_t mode_proc(int32_t argc, int8_t** argv)
{
	lee_module_t* module;

	if (argc < 2) {
		return show_usage(argv[0]);
	}
	
	module = lee_find_module(argv[1]);
	if (NULL == module) {
		util_puts("failed to parse type\n");
		return -1;
	}

	argc -= 2;
	argv += 2;
	module->proc(argc, argv);
	
	return 0;
}

