#if 0
#ifndef NET_TCP_INIT_H
#define NET_TCP_INIT_H

#include "net_init.h"

typedef struct net_tcp_init {
	net_init_t net_init;	
}net_tcp_init_t;

net_unix_tcp_t *net_unix_tcp_init_create();

#endif
#endif
