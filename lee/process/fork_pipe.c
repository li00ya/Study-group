#include <unistd.h>

#include "api/mode.h"
#include "util/string.h"
#include "util/memory.h"

#define BUFSIZE	128

static void fork1(int32_t ret,  int8_t* arg)
{
	util_puts("this child process:\n"
		   "  Read %d bytes:[%s]\n", ret, arg);
}

static int32_t fork_proc(int32_t ac, int8_t** av)
{
	int32_t ret;
	int32_t pipid[2];
	int8_t *p = "hello boy";
	int8_t buf[BUFSIZE];
	pid_t pid;

	util_memset(buf, 0, BUFSIZE);

	if (0 == pipe(pipid)) {
		pid = fork();

		if (-1 == pid) {
			util_puts("Fork Failure\n");
			return -1;
		} else if (0 == pid) {
			ret = read(pipid[0], buf, BUFSIZE);
			fork1(ret, buf);
		} else {
			ret = write(pipid[1], p, util_strlen(p));
			util_puts("Parent process Write %d bytes: %s\n", ret, p);
		}
	}

	return 0;
}

static int32_t fork_usage(void)
{
	util_puts("pipe option:\n\n");

	return 0;
}

static lee_module_t fork_module = {
	.name = "pipe",
	.usage = fork_usage,
	.proc = fork_proc,
};

static void PREP_INIT fork_constructor(void)
{
	lee_module_register(LEE_TYPE_PRO, LEE_PRO_PIPE, &fork_module);
}

static void PREP_EXIT fork_destructor(void)
{
	lee_module_unregister(LEE_TYPE_PRO, LEE_PRO_PIPE);
}

