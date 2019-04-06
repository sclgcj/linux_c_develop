#if 0
#include "net_unix_tcp_init.h"

/**************************************************************************************
* FunctionName   : net_unix_tcp_init()
* Description    : 初始胡tcp unix
* EntryParameter : net,网络对象指针
* ReturnValue    : 返回错误码
**************************************************************************************/
static int32_t net_unix_tcp_init(net_t *net)
{
	int32_t fd = 0;
	net_unix_tcp_t *net_tcp = (net_unix_tcp_t *)net;

	if (!net)
		return ERROR;

	if (!net_tcp->net_unix.socket_path) {
		DEBUG("no socket path set\n");
		return ERROR;
	}

	net_tcp->net_unix.fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (net_tcp->net_unix.fd <= 0) {
		DEBUG("socket  error :%s\n", strerror(errno));
		return ERROR;
	}

	return OK;
}

/**************************************************************************************
* FunctionName   : net_unix_tcp_init_create()
* Description    : 创建unix tcp 初始化函数对象
* EntryParameter : None
* ReturnValue    : 成功返回unix tcp对象指针，失败返回NULL
**************************************************************************************/
net_unix_tcp_init_t *net_unix_tcp_init_create()
{
	net_unix_tcp_init_t *tcp = NULL;

	tcp = (net_unix_tcp_init_t *)calloc(1, 
								sizeof(net_unix_tc_t));
	if (!tcp) {
		ERROR("calloc %d bytes error: %s\n", len, strerror(errno));
		return NULL;
	}

	tcp->net_init.init = net_unix_tcp_init;

	return tcp;
}
#endif
