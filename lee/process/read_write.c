#include <unistd.h>

#include "util/string.h"
#include "util/memory.h"
#include "api/mode.h"

#define BUFSIZE	128

/**
**Three file descriptors are enabled when the program runs
@0:stdin
@1:stdout
@2:stderr
**/
int32_t rw_proc(int32_t ac, int8_t *av[])
{
	int32_t ret;
	int8_t buf[BUFSIZE];
	int8_t *p = "hello spring";

	util_memset(buf, 0, BUFSIZE);

	util_puts("input test data:\n");

	ret = read(0, buf, BUFSIZE);
	util_puts("command line data: %s", buf);

	util_puts("write to printf screen\n");
	ret = write(1, p, util_strlen(p));
	util_puts("\nWrote %d bytes\n", ret);

	return 0;
}

static int32_t rw_usage(void)
{
	util_puts("rw(read and write) option:\n\n");

	return 0;
}

static lee_module_t rw_module = {
	.name = "rw",
	.usage = rw_usage,
	.proc = rw_proc,
};

static void PREP_INIT rw_constructor(void)
{
	lee_module_register(LEE_TYPE_PRO, LEE_PRO_DATA, &rw_module);
}

static void PREP_EXIT rw_destructor(void)
{
	lee_module_unregister(LEE_TYPE_PRO, LEE_PRO_DATA);
}

