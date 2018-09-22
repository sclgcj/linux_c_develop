#include "test.h"
#include "linear_table_test.h"

typedef test_t* (*test_create_t)(char *name);

struct test_proj {
	char *name;
	test_create_t create;
};

static struct test_proj g_test[] = {
	{"sqlist", (test_create_t)linear_table_test_create},
};

status_t test_run(char *name)
{
	int32_t i = 0;
	int32_t nlen = 0, tlen = 0;
	int32_t num = sizeof(g_test) / sizeof(struct test_proj);
	status_t ret = OK;
	test_t *test = NULL;

	if (!name)
		return ERROR;

	nlen = strlen(name);
	for (; i < num; i++) {
		tlen = strlen(g_test[i].name);
		if (tlen != nlen)
			continue;
		if (strncmp(name, g_test[i].name, nlen)) 
			continue;
		
		test = g_test[i].create(name);
		if (test->prepare) {
			ret = test->prepare(test);
			if (ret != OK)
				return ret;
		}
		if (test->run) {
			ret = test->run(test);
			if (ret != OK)
				return ret;
		}
		if (test->result) 
			ret = test->result(test);
		break;
	}
	return ret;
}
