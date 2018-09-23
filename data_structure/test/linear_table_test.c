#include "linear_table_test.h"
#include "sqlist.h"

#define LINEAR_ELEM_SIZE 4

typedef linear_table_t *(*create_t)(int32_t elem_size);

struct linear_table_test_proj {
	uint8_t *name;
	create_t create;
};

static struct linear_table_test_proj g_ltable_proj[] = {
	{"sqlist", (create_t)sqlist_create}
};
static int32_t g_linear_table_test_data1[] = {0, 1, 2, 3, 7, 8};
static int32_t g_linear_table_test_data2[] = {4, 5, 6, 9, 10};

static status_t linear_table_test_compare(void *src, void *dst)
{
	if (*(int32_t*)src == *(int32_t*)dst)
		return TRUE;

	return FALSE;
}

static status_t linear_table_test_visit(void *data)
{
	DEBUG("data = %d\n", *(int32_t *)data);

	return OK;
}

static linear_table_t *linear_table_create(char *name)
{
	int32_t i = 0;
	int32_t nlen = 0, tlen = 0;
	int32_t num = sizeof(g_ltable_proj) / sizeof(struct linear_table_test_proj);

	if (!name)
		return NULL;

	nlen = strlen(name);
	for (; i < num; i++) {
		tlen = strlen(g_ltable_proj[i].name);
		if (tlen != nlen)
			continue;
		if (strncmp(name, g_ltable_proj[i].name, nlen))
			continue;
		
		return (linear_table_t *)g_ltable_proj[i].create(LINEAR_ELEM_SIZE);
	}

	DEBUG("no suit proj : %s\n", name);
	return NULL;
}

static status_t linear_table_test_prepare(test_t *test)
{
	int i = 0;
	int num = sizeof(g_linear_table_test_data1) / LINEAR_ELEM_SIZE;
	linear_table_test_t *ltest = (linear_table_test_t *)test;

	if (!ltest)
		return ERROR;

	ltest->linear_table->linear_table_init(ltest->linear_table);
	
	for (i = 0; i < num; i++)
		ltest->linear_table->linear_table_elem_insert(ltest->linear_table, 
													 i + 1, 
													 (void*)&g_linear_table_test_data1[i]);

	return OK;
}

static status_t linear_table_test_run(test_t *test)
{
	int32_t elem = 0, pos = 0, prior = 0, next = 0;
	status_t status = 0; linear_table_test_t *ltest = (linear_table_test_t*)test;
	linear_table_t *ltable = NULL;

	if (!test)
		return ERROR;

	ltable = ltest->linear_table;
	//遍历测试
	ltable->linear_table_traverse(ltable, linear_table_test_visit);
	//长度测试
	DEBUG("len = %d\n", ltable->linear_table_length(ltable));
	//是否为空测试
	status = ltable->linear_table_empty(ltable);
	if (status == OK)
		DEBUG("sqlist is empty\n");
	else
		DEBUG("sqlist is not empty\n");
	//获取元素测试
	ltable->linear_table_elem_get(ltable, 1, (void*)&elem);
	DEBUG("elem 1 is %d\n", elem);
	elem = 0;
	//定位测试	
	elem = 7;
	pos = ltable->linear_table_elem_locate(ltable, (void*)&elem, linear_table_test_compare);
	DEBUG("pos = %d\n", pos);
	//获取前驱测试
	status = ltable->linear_table_elem_prior(ltable, &elem, &prior);
	if (status != OK) 
		DEBUG("linear_table_elem_prior error\n");
	else 
		DEBUG("prior = %d\n", prior);
	//获取后继测试
	status = ltable->linear_table_elem_next(ltable, &elem, &next);
	if (status != OK)
		DEBUG("linear_table_elem_next error\n");
	else
		DEBUG("next = %d\n", next);
	//插入测试
	elem = 20;
	status = ltable->linear_table_elem_insert(ltable, 3, (void*)&elem);
	if (status != OK)
		DEBUG("linear_table_elem_insert error\n");
	else {
		DEBUG("after insert %d at pos 3, the list data are: \n", elem);
		ltable->linear_table_traverse(ltable, linear_table_test_visit);
	}
	//删除测试
	elem = 0;
	status = ltable->linear_table_elem_delete(ltable, 3, (void*)&elem);
	if (status != OK)
		DEBUG("linear_table_elem_delete error\n");
	else {
		DEBUG("after delete %d at pos 3, the list data are: \n", elem);
		ltable->linear_table_traverse(ltable, linear_table_test_visit);
	}
}

static status_t linear_table_test_result(test_t *test)
{
	return OK;
}


linear_table_test_t *linear_table_test_create(char *name) 
{
	linear_table_test_t *ltest = NULL;

	ltest = (linear_table_test_t *)calloc(1, 
								sizeof(linear_table_test_t));
	if (!ltest)
		return NULL;

	ltest->linear_table = linear_table_create(name);
	if (!ltest->linear_table) {
		free(ltest);
		return NULL;
	}

	ltest->test.prepare = linear_table_test_prepare;
	ltest->test.run		= linear_table_test_run;
	ltest->test.result  = linear_table_test_result;

	return ltest;
}


