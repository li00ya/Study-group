#include "util/macro.h"
#include "util/misc.h"
#include "api/mode.h"

/*
* test the macro of calculate bit num
*/
static int32_t test_bit_calcu(int32_t ac, int8_t *av[])
{
	int32_t num = 0, bits = 0;

	util_puts("Input a Integer num to be caluted bis: ");

	while (!scanf("%d", &num)) {
		util_puts("error: re-enter:\n");
		util_fflush();
	}

	bits = FHS32(num);
	util_puts("the input num %d takes up %d bits\n", num, bits);

	return 0;
}

static int32_t bit_usage(void)
{
	util_puts("bit option:\n\n");

	return 0;
}

static lee_module_t bit_module = {
	.name = "bit",
	.usage = bit_usage,
	.proc = test_bit_calcu,
};

static void PREP_INIT ptr_constructor(void)
{
	lee_module_register(LEE_TYPE_MAC, LEE_MAC_BIT, &bit_module);
}

static void PREP_EXIT ptr_destructor(void)
{
	lee_module_unregister(LEE_TYPE_MAC, LEE_MAC_BIT);
}

