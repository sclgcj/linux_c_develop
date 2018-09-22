#include "linear_table_test.h"

int main(int argc, char **argv)
{
	status_t status = OK;
		
	if (argc < 2) {
		DEBUG("usage: %s proj", argv[0]);
		exit(-1);
	}

	status = test_run(argv[1]);
	if (status == OK)
		DEBUG("test %s PASS\n", argv[1]);
	else
		DEBUG("test %s FAIL\n", argv[1]);

	return 0;
}
