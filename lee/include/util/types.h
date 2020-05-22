#ifndef _TYPES_H_
#define _TYPES_H_

#include <stdio.h>
#include <stdint.h>

#ifndef uvoid_t
typedef unsigned long uvoid_t;
#endif

//如果有多个__attribute__((constructor))的不同名称宏定义，可以使用__attribute__((constructor(110)))，来设置宏的优先级
#define PREP_INIT         __attribute__((constructor))
#define PREP_EXIT         __attribute__((destructor))

#endif
