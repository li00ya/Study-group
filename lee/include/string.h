#ifndef __STRING_H__
#define __STRING_H__

#include "types.h"

void* lee_strcpy(void* dst, void* src);

void* lee_strncpy(void* dst, void* src, uint32_t size);

void* lee_strchr(void* src, int8_t c);

void* lee_strrchr(void* src, int8_t c);

void* lee_strstr(void* src, void* pattern);

void* lee_strsep(void** src, void* pattern);

uint32_t lee_strcspn(void* src, void* pattern);

void* lee_strdup(void* src);

int32_t lee_strcmp(void* dst, void* src);

int32_t lee_strncmp(void* dst, void* src, uint32_t size);

int32_t lee_strlen(void* str);

int32_t lee_sprintf(void* buf, const int8_t* fmt, ...);

int32_t lee_snprintf(void* buf, uint32_t size, const int8_t* fmt, ...);

int32_t lee_vsnprintf(void* buf, uint32_t size, const int8_t* fmt, va_list ap);

int32_t lee_sscanf(void* buf, int8_t* fmt, ...);

int32_t lee_puts(int8_t* fmt, ...);

int32_t lee_show_hex(int8_t* title, void* buf, uint32_t total);

#endif
