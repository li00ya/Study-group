#include "api/term.h"
#include "util/memory.h"
#include "util/string.h"
#include "base/strtox.h"
#include "util/math.h"
#include "api/mode.h"
#include "util/misc.h"

#define TOOL_MSG_SIZE 2048

typedef struct
{
	int32_t ident;
	uint8_t total;
	int8_t name[32];
	float salary;
	info_point_t pos;
}test_data_t;

typedef struct
{
	uint8_t buf[TOOL_MSG_SIZE] ALIGN_NUM(32);
	uint16_t length;
	uint16_t flag;
	uint32_t type;
}tool_msg_t;

typedef struct {
	uint32_t msgcnt;
	void* sym;
	uint32_t total;
	
	tool_msg_t ack;
	uint8_t run;
}tool_info_t;

typedef struct tool_symbol
{
	int8_t* name;
	int32_t (*set)(struct tool_symbol* sym, void* addr, void* value);
	int32_t (*get)(struct tool_symbol* sym, void* addr);

	/* the length about name */
	uint16_t len;
	uint16_t offset;

	/*the memory size when the symbol is string */
	uint16_t size;
}tool_symbol_t;

static tool_info_t* tool_info_get(void)
{
	static tool_info_t info_data;

	return &info_data;
}

static test_data_t* test_data_get(void)
{
	static test_data_t test_data;

	return &test_data;
}

static void tool_puts(test_data_t* info)
{
	
	util_puts("info:\n");
	util_puts("\tident: %d, total %d, salary: %f\n", info->ident, info->total, info->salary);
	util_puts("\tname: %-10s, pos: lon: %f, lat: %f, ele: %f\n", info->name, info->pos.x, info->pos.y, info->pos.z);
}

/********************************************************
*	修改不同类型数值的函数
***********************************************************/
static int32_t tool_set_symbol_uint8(tool_symbol_t* sym, void* addr, void* value)
{
	uint8_t* ptr = addr;
	
	*ptr = util_strtol(value, NULL, 0);

	return 0;
}

static int32_t tool_get_symbol_uint8(tool_symbol_t* sym, void* addr)
{
	uint8_t* ptr = addr;
	
	terms_puts("%d\n", *ptr);

	return 0;
}

static int32_t tool_set_symbol_integer(tool_symbol_t* sym, void* addr, void* value)
{
	uint32_t* ptr = addr;
	
	*ptr = util_strtol(value, NULL, 0);

	return 0;
}

static int32_t tool_get_symbol_integer(tool_symbol_t* sym, void* addr)
{
	uint32_t* ptr = addr;
	
	terms_puts("%d\n", *ptr);

	return 0;
}

static int32_t tool_set_symbol_float(tool_symbol_t* sym, void* addr, void* value)
{
	float* ptr = addr;
	
	*ptr = util_strtof(value, NULL);

	return 0;
}

static int32_t tool_get_symbol_float(tool_symbol_t* sym, void* addr)
{
	float* ptr = addr;

	terms_puts("%f\n", *ptr);

	return 0;
}

static int32_t tool_set_symbol_string(tool_symbol_t* sym, void* addr, void* value)
{
	util_strncpy(addr, value, sym->size);

	return 0;
}

static int32_t tool_get_symbol_string(tool_symbol_t* sym, void* addr)
{
	terms_puts("%s\n", addr);

	return 0;
}

static int32_t tool_set_symbol_pos(tool_symbol_t* sym, void* addr, void* value)
{
	info_point_t* pos = addr;
	int8_t* argv[3];
	
	if (3 != term_parse_ex(value, 3, argv, ',')) {
		return -1;
	}

	pos->x = util_strtod(argv[0], NULL);
	pos->y = util_strtod(argv[1], NULL);
	pos->z = util_strtod(argv[2], NULL);
	
	return 0;
}

static int32_t tool_get_symbol_pos(tool_symbol_t* sym, void* addr)
{
	info_point_t* pos = addr;
	
	terms_puts("lon:%f, lat:%f, elevation:%f\n", pos->x, pos->y, pos->z);
	
	return 0;
}


static int32_t tool_set_symbol(int8_t* name, tool_symbol_t* sym, uint32_t total, void* addr)
{
	test_data_t* test = test_data_get();
	int8_t* value;
	uint32_t idx, len;
	
	value = util_strchr(name, '=');
	if (NULL == value) {
		return -1;
	}
	
	len = value - name;
	value++;
	for (idx = 0; idx < total; idx++, sym++) {
		if (len != sym->len || util_strncmp(sym->name, name, len)) {
			continue;
		}
		
		sym->set(sym, addr + sym->offset, value);
		tool_puts(test);
		return 0;
	}
	
	terms_puts("failed find symbol %s\n", name);
	
	return -1;
}

static int32_t tool_get_symbol(int8_t* name, tool_symbol_t* sym, uint32_t total, void* addr)
{
	uint32_t idx, len;
	
	len = util_strlen(name);
	for (idx = 0; idx < total; idx++, sym++) {
		if (len != sym->len || util_strncmp(sym->name, name, len)) {
			continue;
		}
		
		return sym->get(sym, addr + sym->offset);
	}

	terms_puts("failed find symbol %s\n", name);
	
	return -1;
}

#define TOOL_SYMBOL_INIT(token, type, off, sz) \
{\
	.name = (token), .offset = (off), .size = (sz), \
	.set = tool_set_symbol_##type, .get = tool_get_symbol_##type, \
}

static tool_symbol_t tool_syms[] = {
	TOOL_SYMBOL_INIT("ident", integer, offsetof(test_data_t, ident), 0),
	TOOL_SYMBOL_INIT("total", uint8, offsetof(test_data_t, total), 0),
	TOOL_SYMBOL_INIT("salary", float, offsetof(test_data_t, salary), 0),
	TOOL_SYMBOL_INIT("name",  string, offsetof(test_data_t, name), 32),
	TOOL_SYMBOL_INIT("pos", pos, offsetof(test_data_t, pos), 0),
};

static int32_t tool_init_msgcnt(tool_info_t* info)
{
	test_data_t* test = test_data_get();

	test->total = info->msgcnt++ & 0x7f;

	return 0;
}

/***********************************************
************************************************
************************************************/
static int32_t tool_proc_end(void* data, int32_t argc, int8_t** argv)
{
	tool_info_t* info = data;
	uint8_t time;

	info->run = 0;

	if (argc < 2) {
		return 0;
	}
	time = util_strtol(argv[1], NULL, 0);
	util_puts("%u s后退出程序\n", time);
	util_sleep(time);

	return 0;
}

static int32_t tool_proc_sym(void* data, int32_t argc, int8_t** argv)
{
	tool_info_t* info = data;
	tool_msg_t* ack = &info->ack;
	tool_symbol_t* sym = info->sym;
	void* addr = test_data_get();
	int8_t* cmd;
	uint32_t idx, len;

	cmd = argv[1];
	if (argc < 2 || !util_strcmp("help", cmd)) {
		len = util_snprintf(ack->buf, sizeof(ack->buf), "%-10s  help\n", "cmd");
		len += util_snprintf(ack->buf + len, sizeof(ack->buf) - len, "%-10s ", "param");
		for (idx = 0; idx < info->total; idx++) {
			len += util_snprintf(ack->buf + len, sizeof(ack->buf) - len, " %s", sym[idx].name);
		}
		
		len += util_snprintf(ack->buf + len, sizeof(ack->buf) - len, "\n");
		
		return terms_write(ack->buf, len, 0);
	}
	
	if (util_strchr(cmd, '=')) {
		return tool_set_symbol(cmd, info->sym, info->total, addr);
	}
	
	return tool_get_symbol(cmd, info->sym, info->total, addr);
}

static term_cmd_t tool_cmds[] = {
	{.cmd = "set", .alias = "setParam", .proc = tool_proc_sym, .desc = "set value", .usage = "set [name=value]",},
	{.cmd = "get", .alias = "getParam", .proc = tool_proc_sym, .desc = "get value", .usage = "get [name=value]",},
	{.cmd = "exit", .alias = "end of program", .proc = tool_proc_end, .desc = "end of program runing", .usage = "exit value",},
};

static int32_t tool_find_cmd(void* msgid, term_cmd_t** target)
{
	uint32_t total = sizeof(tool_cmds) / sizeof(tool_cmds[0]);
	term_cmd_t* cmd = tool_cmds;
	uint32_t idx;

	for (idx = 0; idx < total; idx++, cmd++) {
		if (!util_strcmp(cmd->cmd, msgid)) {
			*target = cmd;
			return 0;
		}
				
		if (NULL == cmd->alias || util_strcmp(cmd->alias, msgid)) {
			continue;
		}
		*target = cmd;
		return 0;
	}

	return -1;
}


static int32_t tool_cmd_help(tool_info_t* info, int32_t argc, int8_t** argv)
{
	term_cmd_t* cmd = tool_cmds;
	tool_symbol_t* sym = tool_syms;
	uint32_t total = sizeof(tool_cmds) / sizeof(tool_cmds[0]);
	uint32_t sym_len = sizeof(tool_syms) / sizeof(tool_syms[0]);
	uint32_t idx;
	
	for (idx = 0; idx < total; idx++, cmd++) {
		terms_puts("%-10s  %s\n%*s%s\n", cmd->cmd, cmd->desc, 12, "", cmd->usage);
	}
	
	for (idx = 0; idx < sym_len; idx++, sym++) {
		terms_puts("name:\n\t%-10s\n", sym->name);
	}
	
	return 0;
}

/****************************************************
*	控制台读取传参函数
******************************************************/
static int32_t tool_proc_term(tool_info_t* info, uint32_t timeout)
{
	tool_msg_t* msg = &info->ack;
	int32_t len;
	int32_t argc;
	int8_t* argv[TERM_PARAM_NUM_MAX];
	term_cmd_t* cmd;
	
	util_memset(msg, 0, sizeof(*msg));
	len = terms_read(msg->buf, sizeof(msg->buf) - 1, timeout);
	if (len <= 0) {
		return -1;
	}

	argc = term_parse(msg->buf, TERM_PARAM_NUM_MAX, argv);
	if (argc < 1) {
		util_puts("cmd parse\n");
		return -2;
	}

	if (!util_strcmp("help", argv[0])) {
		return tool_cmd_help(info, argc, argv);
	}

	if (tool_find_cmd(argv[0], &cmd)) {
		util_puts("cmd find %s\n", argv[0]);
		return -3;
	}

	tool_init_msgcnt(info);
	cmd->proc(info, argc, argv);

	return 0;
}

static int32_t tool_exit(void)
{
	terms_exit();

	return 0;
}

static int32_t tool_init_data(void)
{
	test_data_t* test = test_data_get();

	test->ident = 12518;
	test->total = 1;
	test->salary = 3.141592653;
	test->pos.x = 110;
	test->pos.y = 120;
	test->pos.z = 119;
	util_memcpy(test->name, "app", sizeof(test->name));

	return 0;
}

static int32_t tool_init_sym(tool_info_t* info, tool_symbol_t* sym, uint32_t total)
{
	uint32_t idx;

	info->sym = sym;
	info->total = total;
	info->run = 1;

	for (idx = 0; idx < total; idx++, sym++) {
		sym->len = util_strlen(sym->name);
	}

	return 0;
}

static int32_t tool_init_term(int8_t* addr)
{
	term_arg_t arg;

	util_memset(&arg, 0, sizeof(arg));
	arg.addr = addr;

	return terms_init(&arg);
}

static int32_t tool_init(int32_t ac, int8_t** av)
{
	tool_info_t* info = tool_info_get();
	int8_t addr[32] = {0};

	util_memset(info, 0, sizeof(*info));

	if (ac < 1) {
		util_memcpy(addr, "test", sizeof(addr));
	} else {
		util_memcpy(addr, av[0], sizeof(addr));
	}

	if (tool_init_term(addr)) {
		util_puts("term init\n");
		tool_exit();
		return -1;
	}

	tool_init_data();
	tool_init_sym(info, tool_syms, ARRY(tool_syms));
	while (info->run) {
		tool_proc_term(info, 10);
	}

	tool_exit();
	return 0;
}

static int32_t usage(void)
{
	util_puts("term option:\n");
	util_puts("\taddr: file name same as term addr\n");

	return 0;
}

static lee_module_t module = {
	.name = "term",
	.usage = usage,
	.proc = tool_init,
};

static void PREP_INIT constructor(void)
{
	lee_module_register(LEE_TYPE_EXP, LEE_EXP_TERM, &module);
}

static void PREP_EXIT destructor(void)
{
	lee_module_unregister(LEE_TYPE_EXP, LEE_EXP_TERM);
}
