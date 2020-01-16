#include <stdio.h>

typedef int 	int32_t;
typedef char 	int8_t;

#define FHS1(a)  ((a) ? 1 : 0)
#define FHS2(a)  (((a) >> 1) ? (1 + FHS1((a) >> 1)) : FHS1((a) & 1))
#define FHS4(a)  (((a) >> 2) ? (2 + FHS2((a) >> 2)) : FHS2((a) & 3))
#define FHS8(a)  (((a) >> 4) ? (4 + FHS4((a) >> 4)) : FHS4((a) & 0xf))
#define FHS16(a) (((a) >> 8) ? (8 + FHS8((a) >> 8)) : FHS8((a) & 0xff))
#define FHS32(a) (((a) >> 16) ? (16 + FHS16((a) >> 16)) : FHS16((a) & 0xffff))

int32_t main(int32_t ac, int8_t *av[])
{
	int32_t num = 0, bits = 0;

	printf("Input a Integer num to be caluted bis: ");

	while (!scanf("%d", &num))
		scanf("%*[^\n]");

	bits = FHS32(num);
	printf("the input num %d takes up %d bits\n", num, bits);

	return 0;
}
