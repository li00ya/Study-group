#include "util/types.h"
#include "util/string.h"
#include "util/memory.h"
#include "api/mode.h"

typedef struct {
	int* p;
}test_ptr_t;

typedef struct {
	int a;
	test_ptr_t b;
}test_type_t;

static void test_tt(test_type_t* tt, int* a)
{
	tt->b.p = a + 3;
}

/*
**describe:
**	复制结构体数据，结构体中的指针指向的地址仍是原地址
*/
static void test_demo1(void)
{
	int32_t a[10];
	test_type_t m, n;

	a[3] = 6;
	test_tt(&m, a);
	util_memcpy(&n, &m, sizeof(m));//n的指针也指向a[3]的地址
	a[3] = 10;

	util_puts("n ptr %d\n", *n.b.p);
	
}

/*
**describe:
**	局部变量直接赋值给指针，导致指针指向内容不确定
*/
static void test_mode2(int8_t* p)
{
	int8_t c = 'x';

	p = &c;
}

static int32_t ptr_proc(int32_t argc, int8_t** argv)
{
	int8_t p = 0;

	test_demo1();

	test_mode2(&p);
	util_puts(" p 【%c】\n", p);

	return 0;
}

static int32_t ptr_usage(void)
{
	util_puts("ptr option:\n\n");

	return 0;
}

static lee_module_t ptr_module = {
	.name = "ptr",
	.usage = ptr_usage,
	.proc = ptr_proc,
};

static void PREP_INIT ptr_constructor(void)
{
	lee_module_register(LEE_TYPE_EXP, LEE_EXP_PTR, &ptr_module);
}

static void PREP_EXIT ptr_destructor(void)
{
	lee_module_unregister(LEE_TYPE_EXP, LEE_EXP_PTR);
}

