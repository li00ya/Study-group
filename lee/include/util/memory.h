#ifndef _UTIL_MEMORY_H_
#define _UTIL_MEMORY_H_

#include "util/types.h"

void* util_malloc(uint32_t size);

void util_free(void* ptr);

void* util_calloc(uint32_t size, uint32_t num);

void* util_realloc(void* ptr, uint32_t size);

void util_memcpy(void* dst, const void* src, uint32_t size);

int32_t util_memcmp(void* dst, void* src, uint32_t size);

void util_memset(void* ptr, uint32_t key, uint32_t size);

void *util_memmove(void *dest, const void *src, uint32_t n);

#endif
