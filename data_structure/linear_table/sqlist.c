#include "sqlist.h"

status_t sqlist_init(linear_table_t *ltable)
{
	sqlist_t *l = (sqlist_t *)ltable;

	if (!ltable)
		return INFEASIBLE;
	if (l->elem_size == 0)
		return ERROR;
		
	l->elem = (uint8_t*)calloc(LIST_INIT_SIZE, l->elem_size);
	if (!l->elem) 
		exit(OVERFLOW);
	l->length = 0;
	l->listsize = LIST_INIT_SIZE;

	return OK;
}

status_t sqlist_uninit(linear_table_t *ltable)
{
	sqlist_t *l = (sqlist_t *)ltable;
	if (!l)
		return OK;
	if (l->elem)
		free(l->elem);
	l->length = 0;
	l->listsize = 0;
	l->elem_size = 0;

	return OK;
}

status_t sqlist_clear(linear_table_t *ltable)
{
	sqlist_t *l = (sqlist_t *)ltable;
	if (!l)
		return INFEASIBLE;
	memset(l->elem, 0, l->length * l->elem_size);
	l->length = 0;

	return OK;
}

status_t sqlist_empty(linear_table_t *ltable)
{
	sqlist_t *l = (sqlist_t *)ltable;
	if (!l)
		return INFEASIBLE;
	if (l->length > 0)
		return FALSE;

	return OK;
}

status_t sqlist_length(linear_table_t *ltable)
{
	sqlist_t *l = (sqlist_t*)ltable;
	if (!l)
		return INFEASIBLE;

	return l->length;
}

status_t sqlist_elem_get(linear_table_t *ltable, int32_t pos, void *elem)
{
	sqlist_t *l = (sqlist_t*)ltable;
	if (!l)
		return INFEASIBLE;
	if (pos < 0 || pos > sqlist_length(ltable))
		return INFEASIBLE;
	memcpy(elem, l->elem + l->elem_size * (pos - 1), l->elem_size);

	return OK;
}

int32_t sqlist_elem_locate(linear_table_t *ltable, void *e, compare_t compare)
{
	int32_t i = 0;
	sqlist_t *l = (sqlist_t *)ltable;

	if (!l || !compare)
		return INFEASIBLE;

	for (; i < l->length; i++) {
		if (compare(e, l->elem + i * l->elem_size) == TRUE)	
			return (i + 1);
	}

	return 0;
}

static int32_t sqlist_elem_local_locate(linear_table_t *ltable, void *e)
{
	int32_t i = 0;
	int32_t pos = 0;
	sqlist_t *l = (sqlist_t *)ltable;

	for (; i < l->length; i++)	{
		if (!memcmp(e, l->elem + l->elem_size * i, l->elem_size))
			return i + 1;
	}
	
	return 0;
}

status_t sqlist_elem_prior(linear_table_t *ltable, void *e, void *pre_e)
{
	int32_t pos = 0;
	sqlist_t *l = (sqlist_t *)ltable;

	if (!l || !pre_e)
		return INFEASIBLE;

	pos = sqlist_elem_local_locate(ltable, e);
	if (pos == 0 || pos == 1)
		return ERROR;

	//pos - 1是pos在线性表中的位置，
	//pos - 2才是pos的前驱
	memcpy(pre_e, l->elem + l->elem_size * (pos - 2), l->elem_size);

	return OK;
}

status_t sqlist_elem_next(linear_table_t *ltable, void *e, void *next_e)
{
	int32_t pos = 0;
	sqlist_t *l = (sqlist_t *)ltable;
	
	if (!l || !next_e)
		return INFEASIBLE;

	pos = sqlist_elem_local_locate(ltable, e);
	if (pos == 0 || pos == l->length)
		return ERROR;

	memcpy(next_e, l->elem + l->elem_size * pos, l->elem_size);

	return OK;
}

status_t sqlist_elem_insert(linear_table_t *ltable, int32_t pos, void *e)
{
	int32_t i = 0;
	uint8_t *newbase = NULL;
	sqlist_t *l = (sqlist_t*)ltable;

	if (!l || pos < 1 || pos > l->length + 1)
		return INFEASIBLE;

	if (l->length >= l->listsize) {
		newbase = (uint8_t *)realloc(l->elem, (l->listsize + LISTINCREMENT) * l->elem_size);
		memset(l->elem + (l->listsize) * l->elem_size, 0, LISTINCREMENT * l->elem_size);
		l->listsize += LISTINCREMENT;
		l->elem = newbase;
	}
	for	(i = l->length; i > pos - 1; i--) {
		memcpy(l->elem + i * l->elem_size, l->elem + (i - 1) * l->elem_size, l->elem_size);
	}
	memcpy(l->elem + i * l->elem_size, e, l->elem_size);
	l->length += 1;

	return OK;
}

status_t sqlist_elem_delete(linear_table_t *ltable, int32_t pos, void *e)
{
	int32_t i = pos - 1;
	sqlist_t *l = (sqlist_t*)ltable;

	if (!l || !e || pos < 1 || pos > sqlist_length(ltable))
		return INFEASIBLE;

	memcpy(e, l->elem + l->elem_size * i, l->elem_size);

	for (; i < l->length; i++) 
		memcpy(l->elem + i * l->elem_size, \
				l->elem + (i+1) * l->elem_size, l->elem_size);
	memset(l->elem + (i - 1) * l->elem_size, 0, l->elem_size);
	l->length -= 1;

	return OK;
}

status_t sqlist_traverse(linear_table_t *ltable, visit_t visit)
{
	int32_t i = 0;
	sqlist_t *l = (sqlist_t *)ltable;

	if (!l || !visit)
		return INFEASIBLE;

	for (; i < l->length; i++) {
		if (visit(l->elem + i * l->elem_size) != OK)		
			return ERROR;
	}

	return OK;
}

/*
 *
 */
status_t sqlist_merge(linear_table_t *ltable1, linear_table_t *ltable2, 
					  linear_table_t *ltable_out, compare_t compare)
{
	int32_t i = 0, j = 0, k = 0;	
	uint8_t *sqlist1_data = NULL, *sqlist2_data = NULL;
	sqlist_t *sqlist1 = (sqlist_t*)ltable1, 
			 *sqlist2 = (sqlist_t*)ltable2;

	if (!ltable1 || !ltable2 || !ltable_out || !compare)
		return INFEASIBLE;
	if (sqlist1->elem_size != sqlist2->elem_size)
		return INFEASIBLE;

	for (; i < sqlist1->length && j < sqlist2->length;) {
		sqlist1_data = sqlist1->elem + i * sqlist1->elem_size;
		sqlist2_data = sqlist2->elem + j * sqlist2->elem_size;
		if (compare(sqlist1_data, sqlist2_data) == TRUE) {
			sqlist_elem_insert(ltable_out, k++, sqlist1_data);
			i++;
		} else {
			sqlist_elem_insert(ltable_out, k++, sqlist2_data);
			j++;
		}
	}

	for (;i < sqlist1->length;i++) 
		sqlist_elem_insert(ltable_out, k++, sqlist1->elem + i * sqlist1->elem_size);

	for (;j < sqlist2->length;j++)
		sqlist_elem_insert(ltable_out, k++, sqlist2->elem + j * sqlist2->elem_size);

	return OK;
}

sqlist_t *sqlist_create(int32_t elem_size)
{
	sqlist_t *sqlist = NULL;	

	if (elem_size == 0)
		return NULL;

	sqlist = (sqlist_t*)calloc(1, sizeof(sqlist_t));
	if (!sqlist) {
		DEBUG("calloc %d bytes error; %s\n", 
					sizeof(sqlist_t), strerror(errno));
		return NULL;
	}

	sqlist->elem_size = elem_size;

	sqlist->linear_table.linear_table_clear		= sqlist_clear;
	sqlist->linear_table.linear_table_destroy	= sqlist_uninit;
	sqlist->linear_table.linear_table_init		= sqlist_init;
	sqlist->linear_table.linear_table_elem_delete = sqlist_elem_delete;
	sqlist->linear_table.linear_table_elem_get	= sqlist_elem_get;
	sqlist->linear_table.linear_table_elem_insert = sqlist_elem_insert;
	sqlist->linear_table.linear_table_elem_next = sqlist_elem_next;
	sqlist->linear_table.linear_table_elem_locate = sqlist_elem_locate;
	sqlist->linear_table.linear_table_elem_prior = sqlist_elem_prior;
	sqlist->linear_table.linear_table_empty		= sqlist_empty;
	sqlist->linear_table.linear_table_length	= sqlist_length;
	sqlist->linear_table.linear_table_merge		= sqlist_merge;
	sqlist->linear_table.linear_table_traverse  = sqlist_traverse;

	return sqlist;
}

void sqlist_destroy(sqlist_t *sqlist)
{
	if (!sqlist)	
		return;
	
	if (sqlist->linear_table.linear_table_destroy)
		sqlist->linear_table.linear_table_destroy(&sqlist->linear_table);
	free(sqlist);
}


