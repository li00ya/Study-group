#include <stdarg.h>

#include "api/term.h"
#include "util/net.h"
#include "util/string.h"
#include "util/memory.h"
#include "util/misc.h"
#include "util/file.h"

#include "base/ctype.h"

typedef struct
{
	int32_t fd;
	uint32_t used;
	net_addr_t addr;
}term_info_t;

enum
{
	TERM_TYPE_SERVER,
	TERM_TYPE_CLI,
	TERM_TYPE_MAX
};

static term_info_t* terms_get(void)
{
	static term_info_t server;

	return &server;
}

static term_info_t* termc_get(void)
{
	static term_info_t client;

	return &client;
}

static int8_t* term_skip_space(int8_t* buf)
{
	while (isspace(*buf)) {
		buf++;
	}
	
	return buf;
}

static int32_t term_get_fd(term_info_t* info)
{
	return info->used ? info->fd : -1;
}

int32_t term_parse_ex(void* buf, int32_t total, int8_t** argv, uint32_t sep)
{
	int32_t idx;
	int8_t* data;

	util_memset(argv, 0, sizeof(*argv) * total);
	
	data = term_skip_space(buf);
	for (idx = 0; data[0] && idx < total;) {
		argv[idx++] = data;
		data = util_strchr(data, sep);
		if (NULL == data) {
			break;
		}
		
		*data = 0;
		data = term_skip_space(data + 1);
	}
	
	return idx;
}

int32_t term_parse(void* buf, int32_t total, int8_t** argv)
{
	return term_parse_ex(buf, total, argv, ' ');
}

int32_t terms_exit(void)
{
	term_info_t* info = terms_get();
	int8_t cmd[64] = {0};

	if (!info->used) {
		return 0;
	}

	if (info->fd >= 0) {
		util_sock_close(info->fd);
	}

	info->used = 0;

	util_snprintf(cmd, sizeof(cmd), "rm -f %s", "/tmp/.term*");
	util_exec(cmd);

	return 0;
}

static int32_t term_init_addr(net_addr_t* addr, uint32_t type, term_arg_t* arg)
{
	int8_t name[32];
	static int8_t* prefix[TERM_TYPE_MAX] = {"s", "c",};
	
	util_snprintf(name, sizeof(name), "/tmp/.term%s_%s", prefix[type], arg->addr);

	util_memset(addr, 0, sizeof(*addr));
	addr->type = NET_FAMILY_UNIX;
	util_memcpy(addr->name, name, sizeof(addr->name));
	
	return 0;
}

int32_t terms_init(term_arg_t* arg)
{
	term_info_t* info = terms_get();
	net_addr_t addr;

	term_init_addr(&addr, TERM_TYPE_SERVER, arg);
	
	util_memset(info, 0, sizeof(*info));
	info->used = 1;
	info->fd = util_sock_open(NET_FAMILY_UNIX, NET_TYPE_DGRAM);
	if (info->fd < 0) {
		terms_exit();
		util_puts("term server sock\n");
		return -2;
	}
	
	if (util_sock_bind(info->fd, &addr)) {
		terms_exit();
		util_puts("term server bind %s\n", addr.name);
		return -2;
	}

	return 0;
}

int32_t terms_puts(int8_t* fmt, ...)
{
	term_info_t* info = terms_get();
	int32_t fd = term_get_fd(info);
	uint8_t buf[1024];
	uint32_t len;
	va_list ap;

	if (fd < 0) {
		return -1;
	}

	va_start(ap, fmt);
	len = util_vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	return util_sock_sendto(fd, buf, len, &info->addr);
}

int32_t terms_write(void* buf, uint32_t len, uint32_t timeout)
{
	term_info_t* info = terms_get();
	int32_t fd = term_get_fd(info);
	
	if (util_sock_poll(fd, SOCK_POLL_OUT, timeout) <= 0) {
		return -1;
	}
	
	return util_sock_sendto(fd, buf, len, &info->addr);
}

int32_t terms_read(void* buf, uint32_t size, uint32_t timeout)
{
	term_info_t* info = terms_get();
	int32_t fd = term_get_fd(info);
	
	if (util_sock_poll(fd, SOCK_POLL_IN, timeout) <= 0) {
		return -1;
	}
	
	return util_sock_recvfrom(fd, buf, size, &info->addr);
}

int32_t termc_exit(void)
{
	term_info_t* info = termc_get();
	int8_t cmd[64] = {0};

	if (!info->used) {
		return -1;
	}

	if (info->fd >= 0) {
		util_sock_close(info->fd);
	}

	info->used = 0;

	util_snprintf(cmd, sizeof(cmd), "rm -f %s", "/tmp/.termc*");
	util_exec(cmd);

	return 0;
}

int32_t termc_init(term_arg_t* arg)
{
	term_info_t* info = termc_get();
	net_addr_t addr;

	
	util_memset(info, 0, sizeof(*info));
	info->used = 1;
	info->fd = util_sock_open(NET_FAMILY_UNIX, NET_TYPE_DGRAM);
	if (info->fd < 0) {
		termc_exit();
		return -2;
	}
	
	term_init_addr(&addr, TERM_TYPE_CLI, arg);
	if (util_sock_bind(info->fd, &addr)) {
		termc_exit();
		return -2;
	}
	
	term_init_addr(&addr, TERM_TYPE_SERVER, arg);
	if (util_sock_connect(info->fd, &addr)) {
		termc_exit();
		return -2;
	}

	return 0;
}

int32_t termc_write(void* buf, uint32_t len, uint32_t timeout)
{
	term_info_t* info = termc_get();
	int32_t fd = term_get_fd(info);
	
	if (util_sock_poll(fd, SOCK_POLL_OUT, timeout) <= 0) {
		return -1;
	}
	
	return util_sock_send(fd, buf, len);
}

int32_t termc_read(void* buf, uint32_t size, uint32_t timeout)
{
	term_info_t* info = termc_get();
	int32_t fd = term_get_fd(info);
	
	if (util_sock_poll(fd, SOCK_POLL_IN, timeout) <= 0) {
		return -1;
	}
	
	return util_sock_recv(fd, buf, size);
}

