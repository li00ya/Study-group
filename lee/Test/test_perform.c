#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

#define	LOOP	10000000

static uint64_t util_time_system_us(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000000ULL + tv.tv_usec;
}

static void test_perform_if(void)
{
	uint32_t cnt = LOOP;
	uint64_t start, end;

	start = util_time_system_us();

	while (cnt--) {
		if (1) {

		} else {

		}
	}

	end = util_time_system_us() - start;
	printf("if statement average time [%lf] us\n", (double)end / LOOP);
}

static void test_perform_ternary(void)
{
	uint32_t cnt = LOOP;
	uint32_t param;
	uint64_t start, end;

	start = util_time_system_us();

	while(cnt--) {
		param = 1 > 2 ? 1 : 0;
	}

	end = util_time_system_us() - start;
	printf("ternary operator average time [%lf] us\n", (double)end / LOOP);
}

static void test_perform_operator(void)
{
	uint32_t cnt = LOOP;
	uint32_t a, c;
	uint64_t start, end;

	a = 10;

	start = util_time_system_us();

	while (cnt--) {
		c = a + 2;
	}

	end = util_time_system_us() - start;
	printf("add operator average time [%lf] us\n", (double)end / LOOP);

	cnt = LOOP;
	start = util_time_system_us();

	while (cnt--) {
		c = a - 2;
	}

	end = util_time_system_us() - start;
	printf("sub operator average time [%lf] us\n", (double)end / LOOP);

	cnt = LOOP;
	start = util_time_system_us();

	while (cnt--) {
		c = 2 * a;
	}

	end = util_time_system_us() - start;
	printf("mul operator average time [%lf] us\n", (double)end / LOOP);

	
	cnt = LOOP;
	start = util_time_system_us();

	while (cnt--) {
		c = a / 2;
	}

	end = util_time_system_us() - start;
	printf("div operator average time [%lf] us\n", (double)end / LOOP);

	cnt = LOOP;
	start = util_time_system_us();

	while (cnt--) {
		c = a >> 1;
	}

	end = util_time_system_us() - start;
	printf("right shift operator average time [%lf] us\n", (double)end / LOOP);

	cnt = LOOP;
	start = util_time_system_us();

	while (cnt--) {
		c = a << 1;
	}

	end = util_time_system_us() - start;
	printf("left shift operator average time [%lf] us\n", (double)end / LOOP);

	cnt = LOOP;
	start = util_time_system_us();

	while (cnt--) {
		c = a % 2;
	}

	end = util_time_system_us() - start;
	printf("mod operator average time [%lf] us\n", (double)end / LOOP);

}

int32_t main(void)
{
	test_perform_if();
	test_perform_ternary();
	test_perform_operator();

	return 0;
}

