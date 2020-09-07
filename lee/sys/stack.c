#include "util/string.h"
#include "util/memory.h"
#include "api/mode.h"

#include <stdlib.h>
#include <getopt.h>

typedef int32_t stack_data_t;

typedef struct
{
	stack_data_t *data;
	uint32_t top;
	uint32_t size;
}stack_o;

typedef struct stackl
{
	stack_data_t data;
	struct stackl *next;
	uint32_t size;
}stack_l;

typedef struct
{
	int32_t (*init)(void **addr, uint32_t size);
	int32_t (*destroy)(void **addr);
	int32_t (*push)(void **addr, stack_data_t data);
	int32_t (*pop)(void **addr, stack_data_t *data);
}func_t;

union
{
	stack_o o;
	stack_l l;
}stack_var;

typedef struct
{
	void *ops;
	void *stack;
}stack_t;

typedef enum
{
	STACK_TYPE_ARRAY,
	STACK_TYPE_LINK,
	STACK_TYPE_MAX,
}stack_enum_t;

static int32_t stack_o_init(void **addr, uint32_t size)
{
	stack_o *stack = *addr;

	if (0 == size) {
		return -1;
	}

	stack->data = (stack_data_t*)malloc(size * sizeof(stack_data_t));
	if (NULL == stack->data) {
		return -2;	
	}
	stack->size = size;
	stack->top = 0;

	return 0;
}

static int32_t stack_o_destroy(void **addr)
{
	stack_o *stack = *addr;

	if (!stack->size) {
		return 0;
	}

	free(stack->data);
	stack->size = 0;
	stack->data = NULL;

	return 0;
}

static int32_t stack_o_push(void **addr, stack_data_t data)
{
	stack_o *stack = *addr;
	if (!stack->size) {
		return -1;
	}

	if (stack->top == stack->size - 1) {
		return -2;
	}

	stack->data[stack->top++] = data;

	util_puts("array buf remain %d, push data %d\n", stack->size - stack->top, stack->data[stack->top - 1]);

	return 0;
}

static int32_t stack_o_pop(void **addr, stack_data_t *data)
{
	stack_o *stack = *addr;
	if (!stack->size) {
		return -1;
	}

	if (stack->top == 0) {
		return -2;
	}

	*data = stack->data[--stack->top];

	util_puts("array buf remain %d, pop data %d\n", stack->size - stack->top, *data);

	return 0;
}

static int32_t stack_l_init(void **addr, uint32_t size)
{
	*addr = (stack_l*)malloc(sizeof(stack_l));
	if (NULL == *addr) {
		return -1;
	}
	((stack_l*)*addr)->size = 0;

	return 0;
}

static int32_t stack_l_destroy(void **addr)
{
	stack_l *d;

	for (d = *addr; d != NULL; d = *addr) {
		*addr = ((stack_l*)*addr)->next;
		free(d);
	}

	return 0;
}

static int32_t stack_l_push(void **addr, stack_data_t data)
{
	stack_l *stack = *addr;

	if (NULL == stack) {
		return -1;
	}

	stack_l *new = (stack_l*)malloc(sizeof(stack_l));
	if (NULL == new) {
		return -2;
	}
	new->data = data;
	new->next = stack;
	new->size = stack->size + 1;
	*addr = new;

	util_puts("link size %d, push data %d\n", new->size, new->data);

	return 0;
}

static int32_t stack_l_pop(void **addr, stack_data_t *data)
{
	stack_l *stack = *addr;

	if (NULL == stack) {
		return -1;
	}

	*data = stack->data;
	*addr = stack->next;
	free(stack);

	util_puts("link size %d, pop data %d\n", ((stack_l*)*addr)->size, *data);
	return 0;
}

static func_t stack_func[STACK_TYPE_MAX] = {
	{.init = stack_o_init, .destroy = stack_o_destroy, .push = stack_o_push, .pop = stack_o_pop,},
	{.init = stack_l_init, .destroy = stack_l_destroy, .push = stack_l_push, .pop = stack_l_pop,},
};

int32_t stack_init(stack_t *stack, uint32_t size, stack_enum_t type)
{
	func_t *ops;

	util_puts("stack type %d\n", type);

	stack->ops = stack_func + type;
	stack->stack = &stack_var;

	ops = stack->ops;

	return ops->init(&stack->stack, size);
}

int32_t stack_destroy(stack_t *stack)
{
	func_t *ops = stack->ops;

	return ops->destroy(&stack->stack);
}

int32_t stack_push(stack_t *stack, stack_data_t data)
{
	func_t *ops = stack->ops;

	return ops->push(&stack->stack, data);
}

int32_t stack_pop(stack_t *stack, stack_data_t *data)
{
	func_t *ops = stack->ops;

	return ops->pop(&stack->stack, data);
}

static int32_t _usage(void)
{
	util_puts("stack option:\n\n");
	util_puts("\t-t [type]	link | array\n");

	return 0;
}

int32_t _proc(int32_t ac, int8_t **av)
{
	stack_t stack;
	stack_data_t data = 10;
	int8_t *option = "t:h";
	uint32_t type = STACK_TYPE_ARRAY;
	int32_t opt;

	ac += 1;
	av -= 1;	//av[0] is name of proc, option start av[1]

	while ((opt = getopt(ac, av, option)) != -1) {
		switch (opt) {
		case 't':
			if (!util_strcmp(optarg, "link")) {
				type = STACK_TYPE_LINK;
			}
			break;
		case 'h':
		default:
			return _usage();
		}
	}

	stack_init(&stack, 10, type);
	stack_push(&stack, data);
	data = 11;
	stack_push(&stack, data);
	data = 12;
	stack_push(&stack, data);

	stack_pop(&stack, &data);

	stack_destroy(&stack);

	return 0;
}

static lee_module_t _module = {
	.name = "stack",
	.usage = _usage,
	.proc = _proc,
};

static void PREP_INIT _constructor(void)
{
	lee_module_register(LEE_TYPE_PRO, LEE_PRO_STACK, &_module);
}

static void PREP_EXIT _destructor(void)
{
	lee_module_unregister(LEE_TYPE_PRO, LEE_PRO_STACK);
}
