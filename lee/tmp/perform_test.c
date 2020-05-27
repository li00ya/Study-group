#include "util/time.h"
#include "util/string.h"
#include "api/mode.h"

#define	LOOP	10000000

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
	util_puts("if statement average time [%lf] us\n", (double)end / LOOP);
}

static void test_perform_ternary(void)
{
	uint32_t cnt = LOOP;
	int32_t param;
	uint64_t start, end;

	start = util_time_system_us();

	while(cnt--) {
		param = 1 > 2 ? 1 : 0;
	}

	end = util_time_system_us() - start;
	util_puts("param: %d\n", param);
	util_puts("ternary operator average time [%lf] us\n", (double)end / LOOP);
}

static void test_perform_operator(void)
{
	uint32_t cnt = LOOP;
	uint32_t a, c;
	uint64_t start, end;

	a = 10;
	c = 0;

	start = util_time_system_us();

	while (cnt--) {
		c = a + 2;
	}

	util_puts("c: %d\n", c);
	end = util_time_system_us() - start;
	util_puts("add operator average time [%lf] us\n", (double)end / LOOP);

	cnt = LOOP;
	start = util_time_system_us();

	while (cnt--) {
		c = a - 2;
	}

	end = util_time_system_us() - start;
	util_puts("sub operator average time [%lf] us\n", (double)end / LOOP);

	cnt = LOOP;
	start = util_time_system_us();

	while (cnt--) {
		c = 2 * a;
	}

	end = util_time_system_us() - start;
	util_puts("mul operator average time [%lf] us\n", (double)end / LOOP);

	
	cnt = LOOP;
	start = util_time_system_us();

	while (cnt--) {
		c = a / 2;
	}

	end = util_time_system_us() - start;
	util_puts("div operator average time [%lf] us\n", (double)end / LOOP);

	cnt = LOOP;
	start = util_time_system_us();

	while (cnt--) {
		c = a >> 1;
	}

	end = util_time_system_us() - start;
	util_puts("right shift operator average time [%lf] us\n", (double)end / LOOP);

	cnt = LOOP;
	start = util_time_system_us();

	while (cnt--) {
		c = a << 1;
	}

	end = util_time_system_us() - start;
	util_puts("left shift operator average time [%lf] us\n", (double)end / LOOP);

	cnt = LOOP;
	start = util_time_system_us();

	while (cnt--) {
		c = a % 2;
	}

	end = util_time_system_us() - start;
	util_puts("mod operator average time [%lf] us\n", (double)end / LOOP);

}

static int32_t perf_proc(int32_t ac, int8_t** av)
{
	test_perform_if();
	test_perform_ternary();
	test_perform_operator();

	return 0;
}

static int32_t test_usage(void)
{
	util_puts("perform option:\n\n");

	return 0;
}

static lee_module_t perform_module = {
	.name = "perform",
	.usage = test_usage,
	.proc = perf_proc,
};

static void PREP_INIT perform_constructor(void)
{
	lee_module_register(LEE_TYPE_TEST, LEE_TEST_PF, &perform_module);
}

static void PREP_EXIT perform_destructor(void)
{
	lee_module_unregister(LEE_TYPE_TEST, LEE_TEST_PF);
}

