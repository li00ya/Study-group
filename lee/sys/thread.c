/******************************************************
* @File Name: thread.c
* @Author: lee
* @Version: L1
* @Create Time: 2020/8/7
* @Description: A thread example
* @History:
*/

#include <stdlib.h>
#include <pthread.h>

#include "api/mode.h"
#include "util/string.h"
#include "util/misc.h"
#include "util/memory.h"

typedef struct
{
	int8_t name[512];
	int32_t show_time;
}thread_arg_t;

void thread_show(void* arg)
{
	thread_arg_t* data = arg;
	uint32_t idx;

	for (idx = 0; idx < data->show_time; idx++) {
		util_usleep(500 * 1000);
		util_puts("%s: %d\n", data->name, idx);
	}
}

static int32_t _proc(int32_t argc, int8_t** argv)
{
	pthread_t th1, th2;
	thread_arg_t arg1, arg2;

	util_memset(&arg1, 0, sizeof(arg1));
	util_memset(&arg2, 0, sizeof(arg2));

	util_sprintf(arg1.name, "%s", "thread1");
	arg1.show_time = 20;
	util_sprintf(arg2.name, "%s", "thread2");
	arg2.show_time = 18;

	//attar set NULL, means use default thread attar arguments
	if (pthread_create(&th1, NULL, (void*)&thread_show, (void*)&arg1)) {
		util_puts("thread1 create error\n");
		return -1;
	}
	util_puts("thread1 create success\n");
	
	if (pthread_create(&th2, NULL, (void*)&thread_show, (void*)&arg2)) {
		util_puts("thread2 create error\n");
		return -1;
	}
	util_puts("thread2 create success\n");
	
	if (pthread_join(th1, NULL)) {
		util_puts("cannot join thread1\n");
	}
	util_puts("thread1 has destroy\n");
	
	if (pthread_join(th2, NULL)) {
		util_puts("cannot join thread2\n");
	}
	util_puts("thread2 has destroy\n");


	return 0;
}

static int32_t _usage(void)
{
	util_puts("thread option:\n\n");

	return 0;
}

static lee_module_t _module = {
	.name = "thread",
	.usage = _usage,
	.proc = _proc,
};

static void PREP_INIT _constructor(void)
{
	lee_module_register(LEE_TYPE_PRO, LEE_PRO_TH, &_module);
}

static void PREP_EXIT _destructor(void)
{
	lee_module_unregister(LEE_TYPE_PRO, LEE_PRO_TH);
}

