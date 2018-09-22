#ifndef LINEAR_TABLE_TEST_H
#define LINEAR_TABLE_TEST_H

#include "test.h"
#include "linear_table.h"

struct linear_table_test;
typedef struct linear_table_test linear_table_test_t;
struct linear_table_test {
	test_t test;	
	linear_table_t *linear_table;
};

linear_table_test_t *linear_table_test_create(char *name);
void linear_table_test_destroy(linear_table_test_t *ltable_test);

#endif
