#include <unistd.h>

#define __USE_XOPEN
#include <time.h>
#include <sys/time.h>

#include "util/time.h"
#include "util/memory.h"

uint64_t util_time_relative_us(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);

    return ts.tv_sec * 1000000ULL + ts.tv_nsec / 1000;
}

uint64_t util_time_relative_ms(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);

    return ts.tv_sec * 1000ULL + ts.tv_nsec / 1000000;
}

uint64_t util_time_relative_sec(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);

    return ts.tv_sec;
}

uint64_t util_time_system_us(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000000ULL + tv.tv_usec;
}

uint64_t util_time_system_ms(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000ULL + tv.tv_usec / 1000;
}

uint64_t util_time_system_sec(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return tv.tv_sec;
}

static void util_tm_parse(struct tm* tm, util_time_t* time)
{
	time->year = tm->tm_year + 1900;
	time->month = tm->tm_mon + 1;
	time->day = tm->tm_mday;
	time->hour = tm->tm_hour;
	time->minute = tm->tm_min;
	time->second = tm->tm_sec;
}

static void util_tm_build(struct tm* tm, util_time_t* time)
{
	util_memset(tm, 0, sizeof(*tm));

	tm->tm_year = time->year - 1900;
	tm->tm_mon = time->month - 1;
	tm->tm_mday = time->day;
	tm->tm_hour = time->hour;
	tm->tm_min = time->minute;
	tm->tm_sec = time->second;
}

int32_t util_time_get(util_time_t* time)
{
	struct timeval tv;
	struct tm tm;

	gettimeofday(&tv, NULL);
	localtime_r(&tv.tv_sec, &tm);

	util_tm_parse(&tm, time);

	return 0;
}

int32_t util_time_set(util_time_t* time)
{
	struct timeval tv;
	struct tm tm;

	util_tm_build(&tm, time);
	tv.tv_usec = 0;
	tv.tv_sec = mktime(&tm);

	settimeofday(&tv, NULL);

	return 0;
}

int32_t util_time_getzone(uint8_t* zone)
{
	return -1;
}

int32_t util_time_setzone(uint8_t* zone)
{
	return -1;
}

int32_t util_time_parse(void* str, util_time_t* time)
{
	struct tm tm;

	util_memset(&tm, 0, sizeof(tm));
	if (NULL == strptime(str, "%F %T", &tm)) {
		return -1;
	}
	
	util_tm_parse(&tm, time);
	
	return 0;
}

int32_t util_time_build(void* str, uint32_t size, util_time_t* time)
{
	struct tm tm;

	util_tm_build(&tm, time);

	return strftime(str, size, "%F %T", &tm);
}

int32_t util_time_strpsec(void* str, uint32_t size, uint32_t* second)
{
	struct tm tm;
	time_t offset;
	
	if (NULL == strptime("1970-01-01 00:00:00", "%F %T", &tm)) {
		return -1;
	}
	offset = mktime(&tm);
	
	if (NULL == strptime(str, "%F %T", &tm)) {
		return -1;
	}
	
	*second = mktime(&tm) + offset;
	
	return 0;
}

int32_t util_time_strfsec(void* str, uint32_t size,  uint32_t second)
{
	struct tm tm;
	time_t time = second;
	
	gmtime_r(&time, &tm);

	return strftime(str, size, "%F %T", &tm);
}

