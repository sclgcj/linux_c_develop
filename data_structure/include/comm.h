#ifndef COMM_H
#define COMM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <error.h>
#include <errno.h>
#include <sys/un.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>

typedef int32_t status_t;
//元素比较回调函数
typedef status_t (*compare_t)(void *src, void *dst);
//元素访问回调函数
typedef status_t (*visit_t)(void *data);

//调试打印
#define DEBUG(fmt, args...) fprintf(stderr, "[%s:%d] -- "fmt, __FUNCTION__, __LINE__, ##args)
#define ERROR(fmt, args...) fprintf(stderr, "[%s:%d] -- "fmt, __FUNCTION__, __LINE__, ##args)

#define TRUE		1
#define FALSE		0
#define OK			1
#define	ERR			0
#define INFEASIBLE	-1
#define OVERFLOW	-2

#endif
