#ifndef __UTIL_NET_H__
#define __UTIL_NET_H__

#include "util/types.h"

enum
{
	NET_FAMILY_IPV4,
	NET_FAMILY_IPV6,
	NET_FAMILY_PACKET,
	NET_FAMILY_UNIX,
	NET_FAMILY_MAX
};

enum
{
	NET_TYPE_STREAM,
	NET_TYPE_DGRAM,
	NET_TYPE_RAW,
};

typedef struct
{
	union {
		int8_t name[32];
		uint32_t addr[4];
	};
	uint8_t type;
	uint8_t ifindex;
	uint16_t port;
}net_addr_t;

int32_t util_sock_open_ex(uint32_t family, uint32_t type, uint16_t prot);

int32_t util_sock_open(uint32_t family, uint32_t type);

void util_sock_close(int32_t fd);

int32_t util_sock_bind(int32_t fd, net_addr_t* addr);

int32_t util_sock_connect(int32_t fd, net_addr_t* addr);

int32_t util_sock_accept(int32_t fd, net_addr_t* addr);

int32_t util_sock_listen(int32_t fd, int32_t backlog);

int32_t util_sock_block(int32_t fd, uint32_t block);

int32_t util_sock_send(int32_t fd, void* buf, uint32_t len);

int32_t util_sock_recv(int32_t fd, void* buf, uint32_t len);

int32_t util_sock_sendto(int32_t fd, void* buf, uint32_t len, net_addr_t* addr);

int32_t util_sock_recvfrom(int32_t fd, void* buf, uint32_t len, net_addr_t* addr);

typedef struct
{
	uint32_t ifindex;
	uint8_t mac[6];
	uint8_t alive;

}net_info_t;
int32_t util_sock_get_dev_ifindex(int32_t fd, int8_t* devname, net_info_t* info);

int32_t util_sock_get_devmac(int32_t fd, int8_t* devname, net_info_t* info);

int32_t util_sock_bind_dev(int32_t fd, int8_t* devname);

int32_t util_sock_get_devstatus(int32_t fd, int8_t* devname, net_info_t* info);

int32_t util_sock_set_size(int32_t fd, uint32_t recv_size, uint32_t send_size);

int32_t util_sock_get_addr(int32_t fd, int8_t* devname, net_addr_t* addr);

#define SOCK_POLL_IN    1
#define SOCK_POLL_OUT   2

/* timeout unit is ms */
int32_t util_sock_poll(int32_t fd, uint32_t flag, uint32_t timeout);

int32_t util_poll_size(void);
int32_t util_poll_build(void* data, uint32_t idx, int32_t fd, uint32_t flag);
int32_t util_poll_parse(void* data, uint32_t idx, uint32_t* flag);
int32_t util_poll_group(void* data, uint32_t total, uint32_t timeout);

#define SOCK_REUSE_ADDR  1
#define SOCK_REUSE_PORT  2
int32_t util_sock_reuse(int32_t fd, uint32_t flag);

int32_t util_sock_parse_addr(void* ip, uint16_t port, net_addr_t* addr);
int32_t util_sock_build_addr(void* ip, uint32_t size, uint16_t* port, net_addr_t* addr);

uint32_t util_htonl(uint32_t hostlong);
uint16_t util_htons(uint16_t hostshort);
uint32_t util_ntohl(uint32_t netlong);
uint16_t util_ntohs(uint16_t netshort); 

#endif
