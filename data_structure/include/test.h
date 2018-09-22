#ifndef TEST_H
#define TEST_H

#include "comm.h"

struct test;
typedef struct test test_t;

struct test {
	status_t (*prepare)(test_t *test);	//准备测试数据
	status_t (*run)(test_t *test);		//执行测试
	status_t (*result)(test_t *test);	//输出测试结果, 也可并入run中直接输出
};

status_t test_run(char *name);
#endif
