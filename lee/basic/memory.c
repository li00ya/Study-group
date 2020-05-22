#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util/types.h"

void* util_malloc(uint32_t size)
{
	return malloc(size);
}

void util_free(void* ptr)
{
	free(ptr);
}

void* util_calloc(uint32_t size, uint32_t num)
{
	return calloc(size, num);
}

void* util_realloc(void* ptr, uint32_t size)
{
	return realloc(ptr, size);
}

void util_memcpy(void* dst, const void* src, uint32_t size)
{
	memcpy(dst, src, size);
}

int32_t util_memcmp(void* dst, void* src, uint32_t size)
{
	return memcmp(dst, src, size);
}

void util_memset(void* ptr, uint32_t key, uint32_t size)
{
	memset(ptr, key, size);
}


void *util_memmove(void *dest, const void *src, uint32_t n)
{
	return memmove(dest, src, n);
}
