#ifndef _API_TERM_H_
#define _API_TERM_H_

#include "util/types.h"

#define TERM_PARAM_NUM_MAX          10

int32_t term_parse(void* buf, int32_t total, int8_t** argv);
int32_t term_parse_ex(void* buf, int32_t total, int8_t** argv, uint32_t sep);

typedef struct
{
	uint32_t argc;
	int8_t* cmd;
	int8_t* alias;
	int8_t* desc;
	int8_t* usage;
	int32_t (*proc)(void* data, int32_t argc, int8_t** argv);
}term_cmd_t;

typedef struct
{
	void* addr;
	uint32_t port;
}term_arg_t;

/* the prefix terms means term server */
int32_t terms_exit(void);

int32_t terms_init(term_arg_t* arg);

int32_t terms_write(void* buf, uint32_t len, uint32_t timeout);

int32_t terms_read(void* buf, uint32_t size, uint32_t timeout);

int32_t terms_puts(int8_t* fmt, ...);

/* the prefix termc means term client */
int32_t termc_exit(void);

int32_t termc_init(term_arg_t* arg);

int32_t termc_write(void* buf, uint32_t len, uint32_t timeout);

int32_t termc_read(void* buf, uint32_t size, uint32_t timeout);

#endif

