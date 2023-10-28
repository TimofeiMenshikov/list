#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "include/list.h"


#define START_TEST()      																\
	err_t return_code = NO_ERROR;														\
																						\
	printf("___________________________________________________________________\n");	\
																						\
	printf("start %s\n", __func__);														\
																						\
	struct List list;																	\
																						\
	return_code |= list_init(&list, START_SIZE);										\
																						\
	return_code |= list_dump(&list);													\

static err_t test_list_1()
{
	START_TEST()

	return_code |= add_elem_in_tail(&list, 100);

	return_code |= list_dump(&list);

	return_code |= add_elem_in_tail(&list, 200);

	return_code |= list_dump(&list);

	return_code |= add_elem_in_head(&list, 50);

	return_code |= list_dump(&list);

	return_code |= add_elem_after_position(&list, 250, 2);

	return_code |= list_dump(&list);

	return_code |= list_dtor(&list);

	print_list_error(return_code);

	printf("____________________________________________________________________\n");

	return NO_ERROR;
}

static err_t test_cycle_error()
{
	START_TEST()

	list.head = 1;
	list.next[1] = 2;
	list.next[2] = 1;

	find_free_positions_list_with_cycle(&list);

	printf("____________________________________________________________________\n");

	return NO_ERROR;
}


static err_t test_free_list()
{
	START_TEST()

	list.head = 2;

	list.tail = 3;

	(list.data)[1] = 8;

	(list.next)[1] = 3;

	(list.prev)[1] = 2;

	(list.data)[2] = 9;

	(list.next)[2] = 1;

	(list.prev)[2] = 0;

	(list.data)[3] = 13;

	(list.next)[3] = 0;

	(list.prev)[3] = 1;
	

	find_free_positions_list_with_cycle(&list);

	list_dump(&list);

	free_cell(&list, 2);

	list_dump(&list);

	free_cell(&list, 1);

	list_dump(&list);

	free_cell(&list, 3);

	list_dump(&list);

	printf("____________________________________________________________________\n");

	return NO_ERROR;
}



int main(int argc, char* argv[])
{
	for (int test_number = 1; test_number < argc; test_number++)
	{
		if (strcmp(argv[test_number], "test_list_1") == 0)
		{
			test_list_1();
		}
		if (strcmp(argv[test_number], "test_cycle_error") == 0)
		{
			test_cycle_error();
		}
		if (strcmp(argv[test_number], "test_free_list") == 0)
		{
			test_free_list();
		}
	}
}