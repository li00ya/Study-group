#ifndef _MODE_H_
#define _MODE_H_

#include "util/types.h"

enum {
	LEE_TYPE_ALG,
	LEE_TYPE_EXP,
	LEE_TYPE_PRO,
	LEE_TYPE_MAC,
	LEE_TYPE_TEST,
	LEE_TYPE_MAX,
};

enum {
	LEE_ALG_CROSS,
	LEE_ALG_GRID,
};

enum {
	LEE_EXP_SYS,
	LEE_EXP_PTR,
	LEE_EXP_TERM,
};

enum {
	LEE_PRO_FIFO,
	LEE_PRO_PIPE,
	LEE_PRO_DATA,
	LEE_MODE_MAX,
};

enum {
	LEE_MAC_BIT,
};

enum {
	LEE_TEST_PF,
};

typedef struct
{
	int8_t* name;
	int32_t (*usage)(void);
	int32_t (*proc)(int32_t argc, int8_t** argv);
}lee_module_t;

void lee_module_register(uint32_t type, uint32_t idx, lee_module_t* module);

void lee_module_unregister(uint32_t type, uint32_t idx);

int32_t mode_proc(int32_t argc, int8_t** argv);

#endif
