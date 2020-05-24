#include "api/mode.h"
#include "util/string.h"
#include "util/types.h"


static lee_module_t* lee_modules[lee_MODE_MAX];
void lee_module_register(uint32_t type, lee_module_t* module)
{
	lee_modules[type] = module;
}

void lee_module_unregister(uint32_t type)
{
	lee_modules[type] = NULL;
}

static lee_module_t* lee_get_module(uint32_t type)
{
	return lee_modules[type];
}

static int32_t show_usage(int8_t* prog)
{
	int32_t idx;
	lee_module_t* module;

	lee_puts("usage %s module [option]\n", prog);
	lee_puts("module name:\n\t");
	for (idx = 0; idx < LEE_MODE_MAX; idx++) {
		module = lee_get_module(idx);
		if (NULL == module) {
			continue;
		}

		lee_puts("%s,", module->name);
	}
	lee_puts("\n");

	for (idx = 0; idx < LEE_MODE_MAX; idx++) {
		module = lee_get_module(idx);
		if (NULL == module) {
			continue;
		}

		module->usage();
	}
	lee_puts("\n");
	
	return 0;
}

static lee_module_t* lee_find_module(int8_t* name)
{
	lee_module_t* module;
	int32_t idx;

	for (idx = 0; idx < LEE_MODE_MAX; idx++) {
		module = lee_get_module(idx);
		if (NULL == module) {
			continue;
		}
		
		if (lee_strcmp(name, module->name)) {
			continue;
		}

		return module;
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
		lee_puts("failed to parse type\n");
		return -1;
	}

	argc -= 2;
	argv += 2;
	module->proc(argc, argv);
	
	return 0;
}

