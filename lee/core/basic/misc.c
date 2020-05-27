#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <signal.h>
#include <sys/sysinfo.h>
#include <string.h>
#include <errno.h>

#include "util/misc.h"
#include "util/memory.h"
#include "util/string.h"

int32_t util_exec(void* prog)
{
	return system(prog);
}

void util_usleep(uint32_t us)
{
	usleep(us);
}

void util_sleep(uint32_t sec)
{
	sleep(sec);
}

void util_fflush(void)
{
	setbuf(stdin, NULL);
}

int32_t util_poll(int32_t fd, uint32_t flag, uint32_t timeout)
{
	struct pollfd entry;

	util_memset(&entry, 0, sizeof(entry));
	entry.fd = fd;
	entry.events = POLL_FLAG_READ == flag ? POLLIN : POLLOUT;

	return poll(&entry, 1, timeout) > 0 ? 0 : -1;
}

int32_t util_get_pid(void)
{
	return getpid();
}

int32_t util_test_pid(uint32_t pid)
{
	if (pid <= 1) {
		return -1;
	}
	
	return kill(pid, 0);
}

int32_t util_get_cpu_info(sys_stat_t* stat)
{
	FILE* fp;
	uint32_t user, nice, sys, idle, io, irq, softirq;
	int8_t* name;
	int8_t line[512];
	int32_t ret;

	fp = fopen("/proc/stat", "rb");
	if (NULL == fp) {
		return -1;
	}

	name = (int8_t*)fgets((void*)line, sizeof(line), fp);
	name = util_strchr(line, ' ');
	if (NULL == name++) {
		fclose(fp);
		return -1;
	}
	
	ret = util_sscanf((void*)name, (int8_t*)"%d %d %d %d %d %d %d",
		&user, &nice, &sys, &idle, &io, &irq, &softirq);
	if (ret < 7) {
		fclose(fp);
		return -1;
	}
	stat->total = user + nice + sys + idle + io + irq + softirq;
	stat->used = stat->total - idle - io;
	stat->cores = 0;
	
	while(fgets((void*)line, sizeof(line), fp)) {
		if ('c' != line[0]) {
			break;
		}
		stat->cores++;
	}
	fclose(fp);
	
	return  0;
}

int32_t util_get_mem_info(sys_stat_t* stat)
{
	struct sysinfo info;
	FILE* fp;
	int8_t line[512];
	uint32_t cached;

	if (sysinfo(&info)) {
		return -1;
	}

	fp = fopen("/proc/meminfo", "rb");
	if (NULL == fp) {
		return -1;
	}
	
	cached = 0;
	while (fgets((void*)line, sizeof(line), fp)) {
		if (sscanf((void*)line, "Cached: %u %*s\n", &cached) > 0) {
			break;
		}
	}
	fclose(fp);
	
	cached *= 1024;
	cached += info.bufferram + info.freeram;
	stat->used = info.totalram - cached;
	stat->total = info.totalram;
	stat->cores = 0;
	
	return 0;
}

void* util_get_error(void)
{
	return strerror(errno);
}

