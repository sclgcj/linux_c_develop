#if 0
#include "net_uninit.h"

/**************************************************************************************
* FunctionName   : net_unix_tcp_uninit()
* Description    : 反初始化unix tcp 套接字
* EntryParameter : net,网络对象指针
* ReturnValue    : 返回错误码
**************************************************************************************/
static int32_t net_unix_tcp_uninit(net_t *net)
{
	net_unix_tcp_t *net_tcp = (net_unix_tcp_t*)net;

	if (!net)
		return ERROR;

	if (net_tcp->net_unix.fd > 0)
		close(net_tcp->net_unix.fd);

	return OK;
}

/**************************************************************************************
* FunctionName   : net_unix_tcp_uninit_create()
* Description    : 创建unix tcp 反初始化对象
* EntryParameter : None
* ReturnValue    : 成功返回unix tcp 反初始化对象指针，失败返回NULL
**************************************************************************************/
net_unix_tcp_uninit_t *net_unix_tcp_uninit_create() {
	net_unix_tcp_uninit_t *tcp = NULL;

	tcp = (net_unix_tcp_uninit_t *)calloc(1, 
									sizeof(net_unix_tcp_uninit));
	if (!tcp) {
		ERROR("calloc %d bytes error: %s\n", len, strerror(errno));
		return NULL;
	}

	tcp->net_uninit.uninit = net_unix_tcp_uninit;

	return tcp; 
}
#endif
