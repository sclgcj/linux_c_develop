#ifndef NET_INIT_H
#define NET_INIT_H

#include "net.h"

typedef struct net_init {
	int32_t (*init)(net_t *net);
}net_init_t;

#endif
