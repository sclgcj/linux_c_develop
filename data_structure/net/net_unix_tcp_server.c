#include "net_unix_tcp.h"

/**************************************************************************************
* FunctionName   : net_unix_tcp_server_start()
* Description    : 启动unix tcp网路服务 
* EntryParameter : net,网络对象指针
* ReturnValue    : 返回错误码
**************************************************************************************/
static int32_t net_unix_tcp_server_start(net_t *net)
{
	int32_t fd = 0, ret = 0;
	struct sockaddr_un in_addr;
	net_unix_tcp_t *net_tcp = (net_unix_tcp_t *)net;

	if (!net)
		return ERR;

	net_address_get(net, (struct sockaddr*)&in_addr, sizeof(in_addr));
	if (in_addr.sun_path[0]) {
		DEBUG("no socket address set\n");
		return ERR;
	}

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd < 0) {
		DEBUG("socket  error :%s\n", strerror(errno));
		return ERR;
	}
	net_fd_set(net, fd);

	ret = listen(fd, 10); 
	if (ret < 0) {
		ERROR("listen error : %s\n", strerror(errno));
		return ERR;
	}

	return OK;
}

/**************************************************************************************
* FunctionName   : net_unix_tcp_create()
* Description    : 创建tcp unix套接字
* EntryParameter : None
* ReturnValue    : tcp unix对象指针
**************************************************************************************/
net_t *net_unix_tcp_server_create() 
{
	net_unix_tcp_t *net_tcp = NULL;

	net_tcp = (net_unix_tcp_t *)calloc(1,
							sizeof(net_unix_tcp_t));
	if (!net_tcp) {
		DEBUG("calloc %d bytes error : %s\n", 
				sizeof(net_unix_tcp_t), 
				strerror(errno));
		return NULL;
	}

	net_start_set((net_t*)net_tcp, net_unix_tcp_server_start);
	net_stop_set((net_t *)net_tcp, net_unix_tcp_stop);
	net_read_set((net_t *)net_tcp, net_unix_tcp_read);
	net_write_set((net_t *)net_tcp, net_unix_tcp_write);
	net_config_set((net_t *)net_tcp, net_unix_tcp_config);
	net_param_analyse_set((net_t*)net_tcp, net_unix_tcp_param);
	net_help_set((net_t *)net_tcp, NULL);

	return (net_t*)net_tcp;
}

