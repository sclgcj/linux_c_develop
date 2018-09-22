#ifndef SQLIST_H
#define SQLIST_H

#define LIST_INIT_SIZE	100		//线性表存储空间的初始分配量
#define LISTINCREMENT	10		//线性表存储空间的分配增量

#include "linear_table.h"

//-----线性表的动态分配顺序存储结构----
typedef struct {
	linear_table_t linear_table;	//线性表操作
	uint8_t		*elem;				//顺序表元素
	int32_t		elem_size;			//元素大小
	int32_t		length;				//当前长度
	int32_t		listsize;			//当前分配的存储容量(以elem_size为单位)
}sqlist_t;

//创建顺序表示线性表
sqlist_t *sqlist_create(int32_t elem_size);
//销毁顺序表示线性表
void sqlist_destroy(sqlist_t *sqlist);

#endif
