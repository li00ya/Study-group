#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdio.h>

#define int8_t	sint8_t
#include <stdint.h>
#undef	int8_t

#define int8_t char

#define ALIGN_NUM(x)	__attribute__((aligned((x))))
#define ARRY(x)			(sizeof(x) / sizeof(x[0]))

//计算结构体成员相对于该结构体的偏移量
#ifndef offsetof
#define offsetof(TYPE, MEMBER)	((uint32_t)(uint64_t)&((TYPE *)0)->MEMBER)
#endif

#ifndef uvoid_t
typedef unsigned long uvoid_t;
#endif

//如果有多个__attribute__((constructor))的不同名称宏定义，可以使用__attribute__((constructor(110)))，来设置宏的优先级
#define PREP_INIT         __attribute__((constructor))
#define PREP_EXIT         __attribute__((destructor))

#endif
