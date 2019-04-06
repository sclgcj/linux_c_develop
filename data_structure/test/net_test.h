#ifndef NET_TEST_H
#define NET_TEST_H
#include "test.h"
#include "net.h"

struct net_test;
typedef struct net_test net_test_t;
struct net_test {
	test_t test;
	net_t *net;
};

net_test_t *net_test_create(char *name);
void net_test_destroy(net_test_t *net_test);

#endif
