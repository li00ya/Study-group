#ifndef _UTIL_MISC_H_
#define _UTIL_MISC_H_


#include "util/types.h"

int32_t util_exec(void* prog);

void util_usleep(uint32_t us);

void util_sleep(uint32_t sec);

#define POLL_FLAG_READ     1
#define POLL_FLAG_WRITE    2

/*
*desc: poll device status
* param:
*   fd      device handle
*   flag    poll event
*   timeout unit is ms
*return:
*   success when zero, otherwise failed
*/
int32_t util_poll(int32_t fd, uint32_t flag, uint32_t timeout);

/*
*desc: get the current process id
*param: none
*return:
*   success when positive, otherwise failed
*/
int32_t util_get_pid(void);

/*
*desc: test the process id to check that process is valid
*param: 
*     pid     the process id
*return:
*   success when zero, otherwise failed
*/
int32_t util_test_pid(uint32_t pid);

typedef struct
{
	uint32_t total;
	uint32_t used;
	uint32_t cores;
}sys_stat_t;

int32_t util_get_cpu_info(sys_stat_t* stat);

int32_t util_get_mem_info(sys_stat_t* stat);

/*output the message to console */
int32_t util_puts(int8_t* fmt, ...);

int32_t util_show_hex(int8_t* title, void* buf, uint32_t total);

void* util_get_error(void);

#endif

