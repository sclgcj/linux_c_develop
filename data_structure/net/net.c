/**************************************************************************************
**** Copyright (C), 2018, xx xx xx xx info&tech Co., Ltd.

* File Name     : adc.c
* Author        :
* Date          : 2018-10-2
* Description   : .C file function description
* Version       : 1.0 
**************************************************************************************/
#include "net.h"
#include "net_unix_tcp.h"
#include "net_unix_tcp_server.h"

//创建网络协议的回调函数
typedef net_t *(*net_protocol_create_t)(uint8_t *name);
//销毁网络协议的回调函数
typedef void (*net_protocol_destroy_t)(net_t *net);

//网络协议
typedef struct net_protocol{
	uint8_t *name;			
	net_protocol_create_t net_protocol_create;
}net_protocol_t;

//网络协议数组
net_protocol_t g_net_protocol[] = {
	{"unix_tcp", (net_protocol_create_t)net_unix_tcp_create},
	{"unix_tcp_server", (net_protocol_create_t)net_unix_tcp_server_create},
};

/**************************************************************************************
* FunctionName   : net_create()
* Description    : 创建网络对象
* EntryParameter : net_name,网络名称
* ReturnValue    : 网络对象指针
**************************************************************************************/
net_t *net_create(uint8_t *net_name)
{
	int32_t i = 0;
	int32_t nlen = 0, tlen = 0;
	int32_t num = sizeof(g_net_protocol) / sizeof(net_protocol_t);
	net_t *net = NULL;

	nlen = strlen(net_name);
	for (; i < num; i++) {
		tlen = strlen(g_net_protocol[i].name);
		if (nlen != tlen)
			continue;
		if (!strncmp(net_name, g_net_protocol[i].name, nlen)) {
			net = g_net_protocol[i].net_protocol_create(net_name);
			break;
		}
	}
	if (!net) 
		return NULL;

	if (net_epoll_create() == OK) 
		return net;

	net_destroy(net);
	return NULL;
}

/**************************************************************************************
* FunctionName   : net_prepare()
* Description    : 启动网络
* EntryParameter : net,网络对象指针, oper,epoll回调函数
* ReturnValue    : 错误码
**************************************************************************************/
/*int32_t net_start(net_t *net, net_epoll_oper_t *oper)
{
	int32_t ret = 0;
	
	if (net->net_oper.net_start) {
		ret = net->net_oper.net_start(net);
		if (ret != OK)
			return ret;
	}

	ret = net_epoll_create();
	if (ret != OK) 
		return ret;

	if (oper) 
		net_epoll_oper_set(oper);

	return net_epoll_data_add(net->fd, EVENT_READ, (void*)net);
}*/

/**************************************************************************************
* FunctionName   : net_run()
* Description    : 运行网络
* EntryParameter : net,网络对象指针
* ReturnValue    : 错误码
**************************************************************************************/

/**************************************************************************************
* FunctionName   : net_destroy()
* Description    : 销毁网络对象
* EntryParameter : net,网络对象指针
* ReturnValue    : None
**************************************************************************************/
void net_destroy(net_t *net)
{
	net->net_oper.net_stop(net);
	if (net->fd > 0)
		close(net->fd);
	if (net->addr)
		free(net->addr);
	net->addr_len = 0;
	free(net);
}

/**************************************************************************************
* FunctionName   : net_address_set()
* Description    : 设置网络地址
* EntryParameter : net,网络对象指针，addr,网络地址指针，addr_len,地址长度
* ReturnValue    : None
**************************************************************************************/
void net_address_set(net_t *net, struct sockaddr *addr, int32_t addr_len)
{
	if (!net)
		return;

	if (net->addr)
		free(net->addr);
	net->addr_len = 0;

	net->addr = (struct sockaddr *)calloc(1, addr_len);
	if (!net->addr) {
		ERROR("calloc %d byte error: %s\n", addr_len, strerror(errno));
		return;
	}
	memcpy(net->addr, addr, addr_len);
	net->addr_len = addr_len;
}

/**************************************************************************************
* FunctionName   : net_address_get()
* Description    : 获取网络地址
* EntryParameter : net,网络对象指针，addr,存放网络地址的指针，addr_len,地址长度
* ReturnValue    : None
**************************************************************************************/
void net_address_get(net_t *net, struct sockaddr *addr, int32_t addr_len)
{
	if (!net)
		return;

	memcpy(addr, net->addr, addr_len);
}

/**************************************************************************************
* FunctionName   : net_addr_len_get()
* Description    : 获取网络地址长度
* EntryParameter : net,网络对象指针，addr_len,地址长度
* ReturnValue    : None
**************************************************************************************/
void net_addr_len_get(net_t *net, int32_t *addr_len)
{
	if (!net)
		return;

	*addr_len = net->addr_len;
}

/**************************************************************************************
* FunctionName   : net_fd_set()
* Description    : 设置网络套接字文件描述符
* EntryParameter : net,网络对象指针, fd,网络套接字文件描述符
* ReturnValue    : None
**************************************************************************************/
void net_fd_set(net_t *net, int32_t fd)
{
	if (!net)
		return;

	net->fd = fd;
}

/**************************************************************************************
* FunctionName   : net_fd_get()
* Description    : 获取网络套接字文件描述符
* EntryParameter : net,网络对象指针, fd,存放网络套接字文件描述符
* ReturnValue    : None
**************************************************************************************/
void net_fd_get(net_t *net, int32_t *fd)
{
	if (!net)
		return;

	*fd = net->fd;
}

/**************************************************************************************
* FunctionName   :	net_start_set()
* Description    :	设置网络启动方法
* EntryParameter :	net,网络对象指针
*					start,网络启动回调函数
* ReturnValue    :	None
**************************************************************************************/
void net_start_set(net_t *net, int32_t (*start)(net_t *net))
{
	net->net_oper.net_start = start;
}

/**************************************************************************************
* FunctionName   :	net_stop_set()
* Description    : 	设置网络结束方法
* EntryParameter : 	net,网络对象指针
*					stop,网络停止回调函数
* ReturnValue    : 	None
**************************************************************************************/
void net_stop_set(net_t *net, int32_t (*stop)(net_t *net))
{
	net->net_oper.net_stop = stop;
}

/**************************************************************************************
* FunctionName   :	net_read_set()
* Description    : 	设置网络获取数据方法
* EntryParameter : 	net,网络对象指针
*					read,网络获取数据回调函数
* ReturnValue    : 	None
**************************************************************************************/
void net_read_set(net_t *net, int32_t (*read)(net_t *net, void *data, int32_t len))
{
	net->net_oper.net_read = read;
}

/**************************************************************************************
* FunctionName   :	net_write_set()
* Description    : 	设置网络结束方法
* EntryParameter : 	net,网络对象指针
*					write,网络写数据回调函数
* ReturnValue    : 	None
**************************************************************************************/
void net_write_set(net_t *net, int32_t (*write)(net_t *net, void *data, int32_t len))
{
	net->net_oper.net_write = write;
}

/**************************************************************************************
* FunctionName   :	net_config_set()
* Description    : 	设置网络结束方法
* EntryParameter : 	net,网络对象指针
*					config,网络配置回调函数
* ReturnValue    : 	None
**************************************************************************************/
void net_config_set(net_t *net, int32_t (*config)(net_t *net, int cmd, void *data, int32_t len))
{
	net->net_oper.net_config = config;
}

/**************************************************************************************
* FunctionName   :	net_param_analyse_set()
* Description    : 	设置网络参数解析方法
* EntryParameter : 	net,网络对象指针
*					param_analyse,网络配置回调函数
* ReturnValue    : 	None
**************************************************************************************/
void net_param_analyse_set(net_t *net, int32_t (*param_analyse)(net_t * net, char **argv, int32_t argc))
{
	net->net_oper.net_param_analyse = param_analyse;
}

/**************************************************************************************
* FunctionName   :	net_help_set()
* Description    : 	设置网络参数解析方法
* EntryParameter : 	net,网络对象指针
*					param_analyse,网络配置回调函数
* ReturnValue    : 	None
**************************************************************************************/
void net_help_set(net_t *net, int32_t (*help)(net_t *net))
{
	net->net_oper.net_help = help;
}
