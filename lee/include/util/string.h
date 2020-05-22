#ifndef __STRING_H__
#define __STRING_H__

#include "util/types.h"

void* util_strcpy(void* dst, void* src);

void* util_strncpy(void* dst, void* src, uint32_t size);

void* util_strchr(void* src, int8_t c);

void* util_strrchr(void* src, int8_t c);

void* util_strstr(void* src, void* pattern);

void* util_strsep(void** src, void* pattern);

uint32_t util_strcspn(void* src, void* pattern);

void* util_strdup(void* src);

int32_t util_strcmp(void* dst, void* src);

int32_t util_strncmp(void* dst, void* src, uint32_t size);

int32_t util_strlen(void* str);

int32_t util_sprintf(void* buf, const int8_t* fmt, ...);

int32_t util_snprintf(void* buf, uint32_t size, const int8_t* fmt, ...);

int32_t util_vsnprintf(void* buf, uint32_t size, const int8_t* fmt, va_list ap);

int32_t util_sscanf(void* buf, int8_t* fmt, ...);

int32_t util_puts(int8_t* fmt, ...);

int32_t util_show_hex(int8_t* title, void* buf, uint32_t total);

#endif
