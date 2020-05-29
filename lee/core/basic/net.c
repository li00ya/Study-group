#define _GNU_SOURCE
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <net/if.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <netpacket/packet.h>
#include <unistd.h>
#include <linux/un.h>
#include <string.h>
#include <errno.h>

#include "util/net.h"
#include "util/string.h"
#include "util/memory.h"

#ifndef SOCK_CLOEXEC
#define SOCK_CLOEXEC	O_CLOEXEC
#endif

#ifndef SOCK_NONBLOCK
#define SOCK_NONBLOCK	O_NONBLOCK
#endif

typedef union
{
	struct sockaddr_in in4;
	struct sockaddr_in6 in6;
	struct sockaddr_ll sll;
	struct sockaddr_un un;
}util_sockaddr_t;

static uint32_t sock_trans_family(uint32_t family)
{

	switch (family) {
	case NET_FAMILY_IPV6:
		return AF_INET6;
	case NET_FAMILY_PACKET:
		return AF_PACKET;
	case NET_FAMILY_UNIX:
		return AF_UNIX;
	default:
		break;
	}
	
	return AF_INET;
}

static uint32_t sock_trans_type(uint32_t type)
{
	if (NET_TYPE_STREAM == type) {
		return SOCK_STREAM;
	}

	if (NET_TYPE_RAW == type) {
		return SOCK_RAW;
	}

	return SOCK_DGRAM;
}

static uint32_t sock_fold_addr(util_sockaddr_t* daddr, uint32_t* addrlen, net_addr_t* saddr)
{
	util_memset(daddr, 0, sizeof(*daddr));
	
	if (NET_FAMILY_UNIX == saddr->type) {
		*addrlen = sizeof(daddr->un);
		daddr->un.sun_family = AF_UNIX;
		util_strncpy(daddr->un.sun_path, saddr->name, sizeof(daddr->un.sun_path));
		return 0;
	}
	
	if (NET_FAMILY_IPV4 == saddr->type) {
		*addrlen = sizeof(daddr->in4);
		daddr->in4.sin_family = AF_INET;
		daddr->in4.sin_port = saddr->port;
		daddr->in4.sin_addr.s_addr = saddr->addr[0];
		return 0;
	}
	
	*addrlen = sizeof(daddr->in6);
	daddr->in6.sin6_family = AF_INET6;
	daddr->in6.sin6_port = saddr->port;
	util_memcpy(&daddr->in6.sin6_addr, saddr->addr, sizeof(daddr->in6.sin6_addr));

	return 0;
}

static uint32_t sock_unfold_addr(util_sockaddr_t* saddr, net_addr_t* daddr)
{
	util_memset(daddr, 0, sizeof(*daddr));

	if (AF_UNIX == saddr->in4.sin_family) {
		daddr->type = NET_FAMILY_UNIX;
		util_strncpy(daddr->name, saddr->un.sun_path, sizeof(daddr->name));

		return 0;
	}
	
	if (AF_INET == saddr->in4.sin_family) {
		daddr->type = NET_FAMILY_IPV4;
		daddr->port = saddr->in4.sin_port;
		daddr->addr[0] = saddr->in4.sin_addr.s_addr;
		
		return 0;
	}
	
	daddr->type = NET_FAMILY_IPV6;
	daddr->port = saddr->in6.sin6_port;
	daddr->ifindex = saddr->in6.sin6_scope_id;
	util_memcpy(daddr->addr, &saddr->in6.sin6_addr, sizeof(daddr->addr));
	
	return 0;
}

int32_t util_sock_open_ex(uint32_t family, uint32_t type, uint16_t prot)
{
	uint32_t flag = SOCK_CLOEXEC | SOCK_NONBLOCK;

	family = sock_trans_family(family);
	type = sock_trans_type(type);
	
	return socket(family, type | flag, prot);
}

int32_t util_sock_open(uint32_t family, uint32_t type)
{
	return util_sock_open_ex(family, type, 0);
}

void util_sock_close(int32_t fd)
{
	close(fd);
}

int32_t util_sock_bind(int32_t fd, net_addr_t* addr)
{
	util_sockaddr_t daddr;
	uint32_t addrlen;

	if (NET_FAMILY_UNIX == addr->type) {
		unlink((void*)addr->name);
	}
	sock_fold_addr(&daddr, &addrlen, addr);

	return bind(fd, (struct sockaddr*)&daddr, addrlen);
}

int32_t util_sock_connect(int32_t fd, net_addr_t* addr)
{
	util_sockaddr_t daddr;
	uint32_t addrlen;

	sock_fold_addr(&daddr, &addrlen, addr);

	if (!connect(fd, (struct sockaddr*)&daddr, addrlen)) {
		return 0;
	}

	return EINPROGRESS == errno ? 0 : -1;
}

int32_t util_sock_accept(int32_t fd, net_addr_t* addr)
{
	util_sockaddr_t daddr;
	socklen_t dlen = sizeof(daddr);
	uint32_t flag = SOCK_CLOEXEC | SOCK_NONBLOCK;
	int32_t child;
	
	child = accept4(fd, (struct sockaddr*)&daddr, &dlen, flag);
	if (child < 0) {
		return -1;
	}

	sock_unfold_addr(&daddr, addr);

	return child;
}

int32_t util_sock_listen(int32_t fd, int32_t backlog)
{
	return listen(fd, backlog);
}

int32_t util_sock_block(int32_t fd, uint32_t block)
{
	uint32_t flag;

	fcntl(fd, F_GETFL, &flag);
	flag &= ~O_NONBLOCK;
	if (!block) {
		flag |= O_NONBLOCK;
	}

	return fcntl(fd, F_SETFL, &flag);
}

int32_t util_sock_send(int32_t fd, void* buf, uint32_t len)
{
	return send(fd, buf, len, 0);
}

int32_t util_sock_recv(int32_t fd, void* buf, uint32_t len)
{
	return recv(fd, buf, len, 0);
}

int32_t util_sock_sendto(int32_t fd, void* buf, uint32_t len, net_addr_t* addr)
{
	util_sockaddr_t daddr;
	uint32_t addrlen;
	
	sock_fold_addr(&daddr, &addrlen, addr);

	return sendto(fd, buf, len, 0, (struct sockaddr*)&daddr, addrlen);
}

int32_t util_sock_recvfrom(int32_t fd, void* buf, uint32_t len, net_addr_t* addr)
{
	util_sockaddr_t saddr;
	socklen_t alen = sizeof(saddr);
	int32_t ret;

	ret = recvfrom(fd, buf, len, 0, (struct sockaddr*)&saddr, &alen);

	sock_unfold_addr(&saddr, addr);
	
	return ret;
}

int32_t util_sock_get_dev_ifindex(int32_t fd, int8_t* devname, net_info_t* info)
{
	struct ifreq ifr;

	util_memset(&ifr, 0, sizeof(ifr));
	util_strncpy(ifr.ifr_name, devname, IFNAMSIZ);
	if (ioctl(fd, SIOCGIFINDEX, &ifr)) {
		return -1;
	}

	info->ifindex = ifr.ifr_ifindex;

	return 0;
}

int32_t util_sock_get_addr(int32_t fd, int8_t* devname, net_addr_t* addr)
{
	struct ifreq ifr;
	util_sockaddr_t saddr;
	
	util_memset(&ifr, 0, sizeof(ifr));
	util_strncpy(ifr.ifr_name, devname, IFNAMSIZ);
	if (ioctl(fd, SIOCGIFADDR, &ifr)) {
		return -1;
	}

	util_memcpy(&saddr, &ifr.ifr_addr, sizeof(saddr));
	sock_unfold_addr(&saddr, addr);

	return 0;
}

int32_t util_sock_get_devmac(int32_t fd, int8_t* devname, net_info_t* info)
{
	struct ifreq ifr;
	
	util_memset(&ifr, 0, sizeof(ifr));
	util_strncpy(ifr.ifr_name, devname, IFNAMSIZ);
	if (ioctl(fd, SIOCGIFHWADDR, &ifr)) {
		return -1;
	}
	
	util_memcpy(info->mac, &ifr.ifr_hwaddr, sizeof(info->mac));

	return 0;
}

int32_t util_sock_get_devstatus(int32_t fd, int8_t* devname, net_info_t* info)
{
	struct ifreq ifr;
	
	util_memset(&ifr, 0, sizeof(ifr));
	util_strncpy(ifr.ifr_name, devname, IFNAMSIZ);
	if (ioctl(fd, SIOCGIFFLAGS, &ifr)) {
		return -1;
	}
	
	info->alive = !!(ifr.ifr_flags & IFF_UP);

	return 0;
}

int32_t util_sock_bind_dev(int32_t fd, int8_t* devname)
{
	util_sockaddr_t daddr;
	socklen_t alen = sizeof(daddr);
	net_info_t info;
	
	if (getsockname(fd, (struct sockaddr*)&daddr, &alen)) {
		return -1;
	}

	if (AF_PACKET != daddr.in4.sin_family) {
		return setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE, devname, util_strlen(devname));
	}
	
	if (util_sock_get_dev_ifindex(fd, devname, &info)) {
		return -1;
	}

	daddr.sll.sll_ifindex = info.ifindex;
	
	return bind(fd, (struct sockaddr*)&daddr, sizeof(daddr));
}

int32_t util_sock_set_size(int32_t fd, uint32_t recv_size, uint32_t send_size)
{
	if (recv_size) {
		setsockopt(fd, SOL_SOCKET, SO_RCVBUFFORCE, &recv_size, sizeof(recv_size));
	}

	if (send_size) {
		setsockopt(fd, SOL_SOCKET, SO_SNDBUFFORCE, &send_size, sizeof(send_size));
	}

	return 0;
}

static uint32_t sock_trans_pollflag(uint32_t flag)
{
	uint32_t flags = 0;

	if (flag & SOCK_POLL_IN) {
		flags |= POLLIN;
	}

	if (flag & SOCK_POLL_OUT) {
		flags |= POLLOUT;
	}

	return flags;
}

int32_t util_sock_poll(int32_t fd, uint32_t flag, uint32_t timeout)
{
	struct pollfd entry;

	util_memset(&entry, 0, sizeof(entry));
	entry.fd = fd;
	entry.events = sock_trans_pollflag(flag);

	if (poll(&entry, 1, timeout) > 0) {
		return 1;
	}
	
	return (entry.revents & POLLERR) ? -1 : 0;
}

int32_t util_poll_size(void)
{
	return sizeof(struct pollfd);
}

int32_t util_poll_build(void* data, uint32_t idx, int32_t fd, uint32_t flag)
{
	struct pollfd* entry = (struct pollfd*)data + idx;
	
	util_memset(entry, 0, sizeof(*entry));
	entry->fd = fd;
	entry->events = sock_trans_pollflag(flag);

	return 0;
}

int32_t util_poll_parse(void* data, uint32_t idx, uint32_t* flag)
{
	struct pollfd* entry = (struct pollfd*)data + idx;
	
	*flag = 0;
	if (entry->revents & (POLLERR | POLLHUP)) {
		return -1;
	}
	
	if (entry->revents & POLLIN) {
		*flag |= SOCK_POLL_IN;
	}

	if (entry->revents & POLLOUT) {
		*flag |= SOCK_POLL_OUT;
	}

	return 0;
}

int32_t util_poll_group(void* data, uint32_t total, uint32_t timeout)
{
	struct pollfd* entry = (struct pollfd*)data;
	
	return poll(entry, total, timeout);
}

int32_t util_sock_reuse(int32_t fd, uint32_t flag)
{
	int32_t val = 1;
	
	if (flag & SOCK_REUSE_ADDR) {
		setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
	}

#ifdef SO_REUSEPORT
	if (flag & SOCK_REUSE_PORT) {
		setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val));
	}
#endif

	return 0;
}

int32_t util_sock_parse_addr(void* ip, uint16_t port, net_addr_t* addr)
{
	int32_t family;
	
	util_memset(addr, 0, sizeof(*addr));
	
	if (NULL != ip && NULL == util_strchr(ip, '.')) {
		addr->type = NET_FAMILY_IPV6;
	} else {
		addr->type = NET_FAMILY_IPV4;
	}
	
	addr->port = htons(port);
	family = NET_FAMILY_IPV4 == addr->type ? AF_INET : AF_INET6;
	if (NULL == ip) {
		return 0;
	}
	
	return inet_pton(family, ip, addr->addr) <= 0 ? -1 : 0;
}

int32_t util_sock_build_addr(void* ip, uint32_t size, uint16_t* port, net_addr_t* addr)
{
	int32_t family;

	if (NULL != port) {
		*port = ntohs(addr->port);
	}

	if (NULL == ip) {
		return 0;
	}
	
	family = NET_FAMILY_IPV4 == addr->type ? AF_INET : AF_INET6;
	return NULL != inet_ntop(family, addr->name, ip, size) ? 0 : -1;
}

uint16_t util_ntohs(uint16_t netshort)
{
	return ntohs(netshort);	
}

uint32_t util_ntohl(uint32_t netlong)
{
	return ntohl(netlong);	
}

uint16_t util_htons(uint16_t hostshort)
{
	return htons(hostshort);	
}

uint32_t util_htonl(uint32_t hostlong)
{
	return htonl(hostlong);	
}
