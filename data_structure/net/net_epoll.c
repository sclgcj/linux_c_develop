#include "net_epoll.h"

struct net_epoll{
	int32_t exit;			//epoll 退出标志
	int32_t epoll_fd;		//epoll 描述符
	net_epoll_oper_t oper;
};

static net_epoll_t *g_cur_epoll = NULL;		//全局的当前epoll对象指针

#define EPOLL_EVENT_MAX  1024				//最大epoll事件监听个数

/**************************************************************************************
* Description    :  控制epoll文件描述符
**************************************************************************************/
static int32_t net_epoll_data_ctl(int32_t sock, int32_t cmd, 
								  int32_t event, void *ptr)
{
	int32_t	ret = 0;
	struct epoll_event epoll_event, *tmp = NULL;

	
	if (ptr) {
		memset(&epoll_event, 0, sizeof(epoll_event));
		epoll_event.events = event;
		epoll_event.data.ptr = ptr;
		tmp = &epoll_event;
	}
	ret = epoll_ctl(g_cur_epoll->epoll_fd, cmd, sock, tmp);
	if (ret < 0) {
		ERROR("epoll_ctl error: %s\n", strerror(errno));
		return ERR;
	}

	return OK;
}

/**************************************************************************************
* FunctionName   : net_epoll_data_add()
* Description    : 向epoll中添加套接字
* EntryParameter : sock,套接字， event,epoll事件，data,epoll保留的数据
* ReturnValue    : 错误码
**************************************************************************************/
int32_t net_epoll_data_add(int32_t sock, int32_t event, void *data)
{
	return net_epoll_data_ctl(sock, EPOLL_CTL_ADD, event, data);
}

/**************************************************************************************
* FunctionName   : net_epoll_data_mod()
* Description    : 修改epoll描述符事件
* EntryParameter : sock,套接字，event,epoll事件，data,epoll数据
* ReturnValue    : 错误码
**************************************************************************************/
int32_t net_epoll_data_mod(int32_t sock, int32_t event, void *data)
{
	return net_epoll_data_ctl(sock, EPOLL_CTL_MOD, event, data);
}

/**************************************************************************************
* FunctionName   : net_epoll_data_del()
* Description    : 删除epoll描述符事件
* EntryParameter : sock,套接字
* ReturnValue    : 错误码
**************************************************************************************/
int32_t net_epoll_data_del(int32_t sock)
{
	return net_epoll_data_ctl(sock, EPOLL_CTL_DEL, 0, NULL);
}

/**************************************************************************************
* FunctionName   : net_epoll_start()
* Description    : 启动epoll循环
* EntryParameter : None
* ReturnValue    : 错误码
**************************************************************************************/
int32_t net_epoll_start()
{
	int32_t i = 0;	
	int32_t ret = 0;
	int32_t num_fds = 0;
	int32_t event_size = 0;
	struct epoll_event event[EPOLL_EVENT_MAX];

	event_size = sizeof(event);

	for (; event_size != 0;) {
		memset(event, 0, event_size);
		//等待epoll事件
		num_fds = epoll_wait(g_cur_epoll->epoll_fd, event, 
							 EPOLL_EVENT_MAX, 1000);
		DEBUG("num_fds = %d\n", num_fds);
		if (num_fds < 0) {
			if (errno == EINTR)
				break;
			ERROR("epoll_wait error: %s\n", strerror(errno));
			break;
		}
		//判断是否退出
		if (g_cur_epoll->exit == 0) {
			DEBUG("exit epoll\n");
			break;
		}
		//处理epoll事件
		for (i = 0; i < num_fds; i++) {
			if ((event[i].events & EPOLLIN || event[i].events & EPOLLERR))	 {
				if (g_cur_epoll->oper.epoll_recv) 
					g_cur_epoll->oper.epoll_recv(event[i].data.ptr);
			} else if (event[i].events & EPOLLOUT) {
				if (g_cur_epoll->oper.epoll_send) 
					g_cur_epoll->oper.epoll_send(event[i].data.ptr);
			}
		}
	}

	return ERR;
}

/**************************************************************************************
* FunctionName   : net_epoll_create()
* Description    : 创建epoll对象
* EntryParameter : None
* ReturnValue    : 错误码
**************************************************************************************/
int32_t net_epoll_create()
{
	net_epoll_t *net_epoll = NULL;

	if (g_cur_epoll)
		return OK;

	net_epoll = (net_epoll_t *)calloc(1, sizeof(net_epoll_t));
	if (!net_epoll) {
		ERROR("calloc %d bytes error: %s\n", 
				sizeof(net_epoll_t), strerror(errno));
		return ERR;
	}
	net_epoll->epoll_fd = epoll_create(1000000);
	if (net_epoll->epoll_fd < 0) {
		free(net_epoll);
		ERROR("create epoll error: %s\n", strerror(errno));
		return ERR;
	}
	g_cur_epoll = net_epoll;

	return OK;
}

/**************************************************************************************
* FunctionName   : net_epoll_destroy()
* Description    : 销毁epoll对象
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void net_epoll_destroy()
{
	if (!g_cur_epoll)
		return;

	free(g_cur_epoll);
}


/**************************************************************************************
* FunctionName   : net_epoll_oper_set()
* Description    : 设置epoll操作回调
* EntryParameter : oper,epoll操作回调对象指针
* ReturnValue    : 错误码
**************************************************************************************/
int32_t net_epoll_oper_set(net_epoll_oper_t *oper)
{
	if (!oper)
		return -EINVAL;
	if (!g_cur_epoll)
		return ERR;

	memcpy(&g_cur_epoll->oper, oper, sizeof(*oper));

	return OK;
}

/**************************************************************************************
* FunctionName   : net_epoll_exit_set()
* Description    : 设置epoll退出标志
* EntryParameter : None
* ReturnValue    : 错误码
**************************************************************************************/
int32_t net_epoll_exit_set()
{
	if (!g_cur_epoll)
		return ERR;

	g_cur_epoll->exit = 1;

	return OK;
}
