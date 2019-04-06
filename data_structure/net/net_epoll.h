#ifndef NET_EPOLL_H
#define NET_EPOLL_H

#include "comm.h"
#include <sys/epoll.h>


/**************************************************************************************
* Description    :  epoll结构
**************************************************************************************/
typedef struct net_epoll net_epoll_t;

/**************************************************************************************
* Description    :  epoll操作
**************************************************************************************/
typedef struct net_epoll_oper {
	int32_t (*epoll_recv)(void *epoll_data);			//epoll 接收回调函数
	int32_t (*epoll_send)(void *epoll_data);			//epoll 发送回调函数
	int32_t (*epoll_err)(int32_t error_num, void *epoll_data); //epoll 出错回调
}net_epoll_oper_t;

#define EVENT_READ  (EPOLLONESHOT | EPOLLIN)			//EPOLL 读事件
#define EVENT_WRITE (EPOLLONESHOT | EPOLLOUT)			//EPOLL 写事件

/**************************************************************************************
* Description    :	epoll 操作
**************************************************************************************/
//向epoll中添加套接字
int32_t net_epoll_data_add(int32_t sock, int32_t event, void *data);
//修改epoll中套接字事件或者数据
int32_t net_epoll_data_mod(int32_t sock, int32_t event, void *data);
//删除epoll中的套接字
int32_t net_epoll_data_del(int32_t sock);
//启动epoll
int32_t net_epoll_start();
//创建epoll
int32_t net_epoll_create();
//销毁epoll
void net_epoll_destroy();
//设置epoll事件回调函数
int32_t net_epoll_oper_set(net_epoll_oper_t *oper);
//设置epoll退出标志
int32_t net_epoll_exit_set();



#endif
