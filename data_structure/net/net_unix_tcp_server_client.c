#include "net_unix_tcp_server_client.h"

/************************************************************************************** * FunctionName   : net_unix_tcp_server_client_create()
* Description    : 创建tcp unix服务器接收到客户端对象
* EntryParameter : None
* ReturnValue    : 网络对象指针
**************************************************************************************/
net_t *net_unix_tcp_server_client_create()
{
	net_unix_tcp_t *net_tcp = NULL;

	net_tcp = (net_unix_tcp_t*)calloc(1, 
								sizeof(net_unix_tcp_t));
	if (!net_tcp) {
		ERROR("calloc %d bytes error: %s\n", 
				sizeof(net_unix_tcp_t), 
				strerror(errno));
		return NULL;
	}

	net_stop_set((net_t *)net_tcp, net_unix_tcp_stop);
	net_read_set((net_t *)net_tcp, net_unix_tcp_read);
	net_write_set((net_t *)net_tcp, net_unix_tcp_write);

	return (net_t*)net_tcp;	
 }
