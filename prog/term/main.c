#include "api/term.h"
#include "util/memory.h"
#include "util/string.h"
#include "util/misc.h"
#include "util/file.h"

static int32_t usage(int8_t* prog)
{
	util_puts("usage: %s addr cmd [param]\n", prog);
	util_puts("addr: v2x\n");
	util_puts("cmd: set|get and so on.use help if not sure\n");
	util_puts("param: name=value1,value2\n");
	
	return 0;
}

int32_t main(int32_t argc, int8_t** argv)
{
	int32_t idx;
	int32_t len;
	term_arg_t arg;
	int8_t buf[2048];
	
	if (argc < 3) {
		return usage(argv[0]);
	}
	
	util_memset(&arg, 0, sizeof(arg));
	arg.addr = argv[1];
	if (termc_init(&arg)) {
		return util_puts("client init failed\n");
	}
	
	for (len = 0, idx = 2; idx < argc; idx++) {
		len += util_snprintf(buf + len, sizeof(buf) - len, "%s ", argv[idx]);
	}
	buf[--len] = 0;
	
	if (termc_write(buf, len, 1000) < 0) {
		util_puts("req cmd failed %s\n", util_get_error());
		return termc_exit();
	}
	
	while (1) {
		len = termc_read(buf, sizeof(buf) - 1, 1000);
		if (len <= 0) {
			return termc_exit();
		}
			
		buf[len] = 0;
		util_puts("%s", buf);
	}
	
	termc_exit();

	return 0;
}
