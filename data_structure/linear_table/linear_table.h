#ifndef LINEAR_TABLE_H
#define LINEAR_TABLE_H

#include "comm.h"

struct linear_table;
typedef struct linear_table linear_table_t;

struct linear_table{
	//初始化线性表
	status_t (*linear_table_init)(linear_table_t *ltable); //销毁线性表
	status_t (*linear_table_destroy)(linear_table_t *ltable);
	//清空线性表
	status_t (*linear_table_clear)(linear_table_t *ltable);
	//判断线性表是否为空
	status_t (*linear_table_empty)(linear_table_t *ltable);
	//获取线性表长度
	status_t (*linear_table_length)(linear_table_t *ltable);
	//获取线性表指定位置元素
	status_t (*linear_table_elem_get)(linear_table_t *ltable, int32_t pos, void *elem);
	//获取制定元素在线性表中的位置
	int32_t  (*linear_table_elem_locate)(linear_table_t *ltable, void *e, compare_t compare);
	//获取指定元素前驱
	status_t (*linear_table_elem_prior)(linear_table_t *ltable, void *e, void *pre_e);
	//获取指定元素后继
	status_t (*linear_table_elem_next)(linear_table_t *ltable, void *e, void *next_e);
	//在指定位置插入元素
	status_t (*linear_table_elem_insert)(linear_table_t *ltable, int32_t pos, void *elem);
	//删除指定位置元素，并将元素内容返回
	status_t (*linear_table_elem_delete)(linear_table_t *ltable, int32_t pos, void *elem);
	//遍历线性表
	status_t (*linear_table_traverse)(linear_table_t *ltable, visit_t visit);
	//线性表合并
	status_t (*linear_table_merge)(linear_table_t *ltable1, linear_table_t *ltable2, 
								   linear_table_t *ltable_out, compare_t compare);
};

#endif
