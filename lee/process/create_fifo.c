#include<sys/types.h>
#include<sys/stat.h>

#include "api/mode.h"
#include "util/string.h"

static int32_t fifo_proc(int32_t ac, int8_t** av)
{
	int32_t ret = mkfifo("/tmp/my_fifo", 0777);
	if (0 == ret) util_puts("my fifo create sucess\n");

	return 0;
}

static int32_t fifo_usage(void)
{
	util_puts("fifo option:\n\n");

	return 0;
}

static lee_module_t fifo_module = {
	.name = "fifo",
	.usage = fifo_usage,
	.proc = fifo_proc,
};

static void PREP_INIT fifo_constructor(void)
{
	lee_module_register(LEE_TYPE_PRO, LEE_PRO_FIFO, &fifo_module);
}

static void PREP_EXIT fifo_destructor(void)
{
	lee_module_unregister(LEE_TYPE_PRO, LEE_PRO_FIFO);
}

