#ifndef _MODE_H_
#define _MODE_H_

#include "util/types.h"

enum {
	LEE_TYPE_ALG,
	LEE_TYPE_EXP,
	LEE_TYPE_PRO,
	LEE_TYPE_MAC,
};

#define lee_MODE_MAX 2048
#define TYPE_OFFSET	7
#define MODE_BUILD(type, value)	\
	(((type) << TYPE_OFFSET) | (value))

#define MODE_VALUE(value)	((value) & 0x7F)
#define MODE_TYPE(TYPE)	(((TYPE) >> TYPE_OFFSET) & 0xF)

enum {
	LEE_ALG_CROSS = MODE_BUILD(LEE_TYPE_ALG, 0),
	LEE_ALG_GRID,
};

enum {
	LEE_EXP_SYS = MODE_BUILD(LEE_TYPE_EXP, 0),
	LEE_EXP_PTR,
};

enum {
	LEE_PRO_FIFO = MODE_BUILD(LEE_TYPE_PRO, 0),
	LEE_PRO_PIPE,
	LEE_PRO_DATA,
};

enum {
	LEE_MAC_BIT = MODE_BUILD(LEE_TYPE_MAC, 0),
};

typedef struct
{
	int8_t* name;
	int32_t (*usage)(void);
	int32_t (*proc)(int32_t argc, int8_t** argv);
}lee_module_t;

void lee_module_register(uint32_t type, lee_module_t* module);

void lee_module_unregister(uint32_t type);

int32_t mode_proc(int32_t argc, int8_t** argv);

#endif
