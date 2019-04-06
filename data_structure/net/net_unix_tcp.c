#include "net_unix_tcp.h"
#include "net_unix.h"

//tcp unix 对象
/**************************************************************************************
* FunctionName   : net_unix_tcp_start()
* Description    : 启动unix tcp网路* EntryParameter : net,网络对象指针
* ReturnValue    : 返回错误码
**************************************************************************************/
int32_t net_unix_tcp_start(net_t *net)
{
	int32_t fd = 0, ret = 0; struct sockaddr_un in_addr;
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
	net_epoll_data_add(fd, EVENT_READ, (void*)net);

	ret = connect(fd, (struct sockaddr *)&in_addr, sizeof(in_addr));
	if (ret < 0) {
		DEBUG("connect %s error :%s\n", 
			  ((struct sockaddr_un*)net_tcp->net_unix.addr)->sun_path, 
			  strerror(errno));
		close(fd);
		net_fd_set(net, -1);
		return ERR;
	}

	return OK;
}

/**************************************************************************************
* FunctionName   : net_unix_tcp_stop()
* Description    : 反初始化unix tcp 套接字
* EntryParameter : net,网络对象指针
* ReturnValue    : 返回错误码
**************************************************************************************/
int32_t net_unix_tcp_stop(net_t *net)
{
	net_unix_tcp_t *net_tcp = (net_unix_tcp_t*)net;

	if (!net)
		return ERR;

	return OK;
}

/**************************************************************************************
* FunctionName   : net_unix_tcp_read()
* Description    : 读取tcp unix 套机子数据
* EntryParameter : net,网络对象指针，data，存放读取的数据， len,指向读取数据长度
* ReturnValue    : 成功返回读取的实际长度，失败返回错误码
**************************************************************************************/
int32_t net_unix_tcp_read(net_t *net, void *data, int32_t len)
{
	int32_t ret = 0, fd = 0;

	if (!net || !data || len <= 0)
		return ERR;

	net_fd_get(net, &fd);
	ret = recv(fd, data, len, 0);
	if (ret <= 0) {
		ERROR("recv %d byte error:%s\n", len, strerror(errno));
		return ERR;
	}

	return OK;
}

/**************************************************************************************
* FunctionName   : net_unix_tcp_write()
* Description    : 向tcp unix套接字写入数据
* EntryParameter : net,网络对象指针，data,指向写入的数据， len,指向写入数据长度
* ReturnValue    : 成功返回写入数据长度，失败返回错误码
**************************************************************************************/
int32_t net_unix_tcp_write(net_t *net, void *data, int32_t len)
{
	int32_t ret = 0, fd = 0;

	if (!net || !data || len <= 0)
		return ERR;

	net_fd_get(net, &fd);
	ret = send(fd, data, len, 0);
	if (ret < 0) {
		ERROR("send %d bytes error %s\n", len, strerror(errno));
		return ERR;
	}

	return OK;
}

/**************************************************************************************
* FunctionName   : net_unix_tcp_config()
* Description    : 配置tcp unix套接字
* EntryParameter : net,网络对象指针，cmd,配置命令, data,配置的数据，len,配置数据长度
* ReturnValue    : 返回发送状态或者长度
**************************************************************************************/
int32_t net_unix_tcp_config(net_t *net, int32_t cmd, void *data, int32_t len)
{
	if (!net || !data || len <= 0)
		return ERR;

	return OK;
}

/**************************************************************************************
* FunctionName   : net_unix_tcp_help()
* Description    : unix tcp 帮助函数
* EntryParameter : net,网络对象指针
* ReturnValue    : 返回发送状态或者长度
**************************************************************************************/
int32_t net_unix_tcp_help(net_t *net, int32_t cmd, void *data, int32_t len)
{
	return OK;
}

/**************************************************************************************
* FunctionName   : net_unix_tcp_param()
* Description    : 设置tcp unix命令行参数
* EntryParameter : net,网络对象指针，argv,参数数组，argc,参数数组个数
* ReturnValue    : 返回发送状态或者长度
**************************************************************************************/
int32_t net_unix_tcp_param(net_t *net, char **argv, int32_t argc)
{
	int32_t c = 0, opt_idx = 0;
	char *opt_args = NULL;
	struct sockaddr_un addr;
	struct option unix_tcp_param[] = {
		{"sock_path", 1, 0, 0},
	};
	net_unix_tcp_t *net_tcp = (net_unix_tcp_t *)net;

	if (!net)
		return ERR;

	while (1) {
		opt_idx = 0;
		c = getopt_long(argc, argv, opt_args, unix_tcp_param, 
						&opt_idx);
		if (c == -1)
			break;
		if (c == 0) {
			if (!strncmp(unix_tcp_param[opt_idx].name, "sock_path", 
						strlen("sock_path"))) 
				net_unix_addr_set(net, optarg);
		}
	}

	return OK;
}

/**************************************************************************************
* FunctionName   : net_unix_tcp_create()
* Description    : 创建tcp unix套接字
* EntryParameter : None
* ReturnValue    : tcp unix对象指针
**************************************************************************************/
net_t *net_unix_tcp_create() 
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

	net_start_set((net_t*)net_tcp, net_unix_tcp_start);
	net_stop_set((net_t *)net_tcp, net_unix_tcp_stop);
	net_read_set((net_t *)net_tcp, net_unix_tcp_read);
	net_write_set((net_t *)net_tcp, net_unix_tcp_write);
	net_config_set((net_t *)net_tcp, net_unix_tcp_config);
	net_param_analyse_set((net_t*)net_tcp, net_unix_tcp_param);
	net_help_set((net_t *)net_tcp, NULL);

	return (net_t*)net_tcp;
}

