#ifndef __UTIL_TIME_H__
#define __UTIL_TIME_H__

#include "util/types.h"

uint64_t util_time_relative_us(void);

uint64_t util_time_relative_ms(void);

uint64_t util_time_relative_sec(void);


uint64_t util_time_system_us(void);

uint64_t util_time_system_ms(void);

uint64_t util_time_system_sec(void);

typedef struct
{
	int32_t year;
	
	/* the value range is 1 -- 12 */
	int32_t month;
	
	/* the value range is 1 -- 31 */
	int32_t day;
	
	/* the value range is 0 -- 23 */
	int32_t hour;
	
	/* the value range is 0 -- 59 */
	int32_t minute;
	int32_t second;
}util_time_t;

int32_t util_time_get(util_time_t* time);
int32_t util_time_set(util_time_t* time);

int32_t util_time_getzone(uint8_t* zone);
int32_t util_time_setzone(uint8_t* zone);

/* the time format is YYYY-mm-dd HH:MM:SS */
int32_t util_time_parse(void* str, util_time_t* time);
int32_t util_time_build(void* str, uint32_t size, util_time_t* time);

/* UTC time */
int32_t util_time_strpsec(void* str, uint32_t size, uint32_t* second);
int32_t util_time_strfsec(void* str, uint32_t size, uint32_t second);

#define SECONDS_TO_2004       (1072915200UL)
#define SECONDS_PER_WEEK		(604800)

#endif
