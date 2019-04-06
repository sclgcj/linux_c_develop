#include "net_test.h"

static status_t net_test_prepare(test_t *test)
{
	return OK;
}

static status_t net_test_run(test_t *test)
{
	return OK;
}

static status_t net_test_result(test_t *test)
{
	return OK;
}

net_test_t *net_test_create(char *name)
{
	net_test_t *ntest = NULL;		

	ntest = (net_test_t *)calloc(1, sizeof(*ntest));
	if (!ntest)
		return NULL;

	ntest->net = net_create(name);
	if (!ntest->net) {
		free(ntest);
		return NULL;
	}

	ntest->test.prepare = net_test_prepare;	
	ntest->test.run		= net_test_run;
	ntest->test.result	= net_test_result;
}
