#if 0
#ifndef NET_UNINIT_H
#define NET_UNINIT_H

#include "net.h"

typedef struct net_uninit {
	int32_t (*uninit)(net_t *net);
}net_uninit_t;

net_unix_tcp_uninit_t *net_unix_tcp_uninit_create();

#endif
#endif
