#include <stdio.h>
#include <string.h>

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
	int a[10];
	test_type_t m, n;

	a[3] = 6;
	test_tt(&m, a);
	memcpy(&n, &m, sizeof(m));//n的指针也指向a[3]的地址
	a[3] = 10;

	printf("n ptr %d\n", *n.b.p);
	
}

/*
**describe:
**	局部变量直接赋值给指针，导致指针指向内容不确定
*/
static void test_mode2(char* p)
{
	char c = 'x';

	p = &c;
}

int main(void)
{
	char p;

	test_demo1();

	test_mode2(&p);
	printf(" p 【%c】\n", p);
	return 0;
}
