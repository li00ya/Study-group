#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "util/string.h"


void* util_strcpy(void* dst, void* src)
{
	return strcpy(dst, src);
}

void* util_strncpy(void* dst, void* src, uint32_t size)
{
	return strncpy(dst, src, size);
}

void* util_strchr(void* src, int8_t c)
{
	return strchr(src, c);
}

void* util_strrchr(void* src, int8_t c)
{
	return strrchr(src, c);
}

void* util_strstr(void* src, void* pattern)
{
	return strstr(src, pattern);
}

void* util_strsep(void** src, void* pattern)
{
	return strsep((int8_t**)src, pattern);
}

uint32_t util_strcspn(void* src, void* pattern)
{
	return strcspn(src, pattern);
}

void* util_strdup(void* src)
{
	return strdup(src);
}

int32_t util_strcmp(void* dst, void* src)
{
	return strcmp(dst, src);
}

int32_t util_strncmp(void* dst, void* src, uint32_t size)
{
	return strncmp(dst, src, size);
}


int32_t util_strlen(void* str)
{
	return strlen(str);
}

int32_t util_sprintf(void* buf, const int8_t* fmt, ...)
{
	va_list ap;
	
	va_start(ap, fmt);
	
	return vsprintf(buf, fmt, ap);
}

int32_t util_snprintf(void* buf, uint32_t size, const int8_t* fmt, ...)
{
	va_list ap;
	
	va_start(ap, fmt);
	
	return vsnprintf(buf, size, fmt, ap);
}


int32_t util_vsnprintf(void* buf, uint32_t size, const int8_t* fmt, va_list ap)
{
	return vsnprintf(buf, size, fmt, ap);
}

int32_t util_sscanf(void* buf, int8_t* fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	return vsscanf(buf, fmt, ap);
}

