#ifndef NET_UNIX_TCP_H
#define NET_UNIX_TCP_H

#include "net.h"

typedef struct net_unix_tcp {
	net_t net_unix;
}net_unix_tcp_t;


//创建unix tcp 套接字
net_t *net_unix_tcp_create();
//启动unix tcp 网络
int32_t net_unix_tcp_start(net_t *net);
//停止unix tcp 网络
int32_t net_unix_tcp_stop(net_t *net);
//读取unix tcp数据
int32_t net_unix_tcp_read(net_t *net, void *data, int32_t len);
//写入unix tcp数据
int32_t net_unix_tcp_write(net_t *net, void *data, int32_t len);
//配置unix tcp 数据
int32_t net_unix_tcp_config(net_t *net, int32_t cmd, void *data, int32_t len);
//解析unix tcp命令行参数
int32_t net_unix_tcp_param(net_t *net, char **argv, int32_t argc);


#endif
