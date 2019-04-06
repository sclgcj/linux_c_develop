#ifndef NET_H
#define NET_H

#include "comm.h"
#include "net_epoll.h"

struct net;
typedef struct net net_t;
struct net_operation;
typedef struct net_operation net_operation_t;

//网络配置命令
enum {
	NET_SEND_BUF_SET,			//设置网络发送缓冲区大小
	NET_SEND_BUF_GET,			//获取网络发送缓冲区大小
	NET_RECV_BUF_SET,			//设置网络接收缓冲区大小
	NET_RECV_BUF_GET,			//获取网络接收缓冲区大小
	NET_SEND_TIMEOUT_SET,		//设置网络发送超时时间
	NET_SEND_TIMEOUT_GET,		//获取网络发送超时时间
	NET_RECV_TIMEOUT_SET,		//设置网络接收超时时间
	NET_RECV_TIMEOUT_GET,		//获取网络接收超时时间
	NET_NONBLOCK_SET,			//设置网络非阻塞套接字	
	NET_BLOCK_SET,				//设置网络阻塞套接字
};

//网络操作接口
struct net_operation {
	int32_t (*net_start)(net_t *net);
	int32_t (*net_stop)(net_t *net);
	int32_t (*net_read)(net_t *net, void *data, int32_t len);
	int32_t (*net_write)(net_t *net, void *data, int32_t len);
	int32_t (*net_config)(net_t *net, int32_t cmd, void *data, int32_t len);
	int32_t (*net_param_analyse)(net_t *net, char **argv, int32_t argc);
	int32_t (*net_help)(net_t *net);
};

struct net {
	net_operation_t net_oper;
	int fd;
	int32_t addr_len;
	struct sockaddr *addr;
};

//创建网络协议
net_t *net_create(uint8_t *net_name);
//销毁网络协议
void net_destroy(net_t *net);
//设置网络套接字文件描述符
void net_fd_set(net_t *net, int32_t fd);
//获取网络套接字文件描述符
void net_fd_get(net_t *net, int32_t *fd);
//设置网络地址(目的地址)
void net_address_set(net_t *net, struct sockaddr *addr, int32_t addr_len);
//获取网络地址
void net_address_get(net_t *net, struct sockaddr *addr, int32_t addr_len);
//获取地址长度
void net_addr_len_get(net_t *net, int32_t *addr_len);
//设置网络启动方法
void net_start_set(net_t *net, int32_t (*start)(net_t *net));
//设置网络停止方法
void net_stop_set(net_t *net, int32_t (*stop)(net_t *net));
//设置网络读取数据方法
void net_read_set(net_t *net, int32_t (*read)(net_t *net, void *data, int32_t len));
//设置网络写数据方法
void net_write_set(net_t *net, int32_t (*write)(net_t *net, void *data, int32_t len));
//设置网络配置方法
void net_config_set(net_t *net, int32_t (*config)(net_t *net, int cmd, void *data, int32_t len));
//设置网络命令行参数解析方法
void net_param_analyse_set(net_t *net, int32_t (*param_analyse)(net_t * net, char **argv, int32_t argc));
//设置网络帮助方法
void net_help_set(net_t *net, int32_t (*help)(net_t *net));


#endif
