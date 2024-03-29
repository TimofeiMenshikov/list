#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdarg.h>

#include "include/list.h"
#include "include/graph.h"
#include <assert.h>


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
																						\
	FILE* html_file = fopen(html_filepath, "w");										\
	assert(html_file);																	\
																						\
	char graph_filepath[MAX_FILENAME_SIZE] = {};										\
																						\
	ssize_t number = 0;																	\


#define END_TEST()																			\
    fclose(html_file);																		\
	RUN_HTML_FILE(graph_folder, html_filename);												\


#define IFRUN_TEST(func, ...) if (strcmp(argv[test_number], #func) == 0) func(__VA_ARGS__)


static err_t test_list_1()
{
	START_TEST()

	

	return_code |= add_elem_after_position(&list, 100, 0);

	return_code |= list_dump(&list);

	CREATE_ONE_GRAPH()


	return_code |= add_elem_after_position(&list, 200, 1);

	return_code |= list_dump(&list);

	CREATE_ONE_GRAPH()



	

	return_code |= add_elem_before_position(&list, 50, 1);

	return_code |= list_dump(&list);

	CREATE_ONE_GRAPH()

	/*ssize_t prev_free = list.free;

	move_elem(&list, 2, list.free);

	CREATE_ONE_GRAPH()

	move_elem(&list, 3, 2);

	CREATE_ONE_GRAPH()

	move_elem(&list, prev_free, 3);*/

	swap_two_elements(&list, 3, 2);

	CREATE_ONE_GRAPH()

	return_code |= add_elem_after_position(&list, 250, 2);

	return_code |= list_dump(&list);

	CREATE_ONE_GRAPH()

	return_code |= add_elem_after_position(&list, 243, 2);

	return_code |= list_dump(&list);



	return_code |= free_cell(&list, 2);

	return_code |= list_dump(&list);

	
	CREATE_ONE_GRAPH()

	return_code |= group_elements(&list);

	CREATE_ONE_GRAPH()

	return_code |= straighten_list_transitions(&list);

	CREATE_ONE_GRAPH()

	return_code |= list_decrease(&list);

	CREATE_ONE_GRAPH()



	return_code |= list_dtor(&list);

	print_list_error(return_code);

	printf("____________________________________________________________________\n");

	END_TEST()

	return NO_ERROR;
}

static err_t test_increase_list()
{
	START_TEST()

	CREATE_ONE_GRAPH()

	list_increase(&list);

	CREATE_ONE_GRAPH()

	END_TEST()

	return NO_ERROR;
}

static err_t test_cycle_error()
{
	START_TEST()

	//list.head = 1;
	list.next[1] = 2;
	list.next[2] = 1;

	find_free_positions_list_with_cycle(&list);

	printf("____________________________________________________________________\n");

	return NO_ERROR;
}


static err_t test_free_list()
{
	START_TEST()

	//list.head = 2;

	//list.tail = 3;

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



#define test_graph_with_for()																\
	START_TEST();																			\
	for (ssize_t number = 0; number < 1; number++)											\
	{																						\
		CREATE_ONE_GRAPH()																	\
	}																						\
																							\
	END_TEST()																				\

	

int main(int argc, char* argv[])
{
	for (int test_number = 1; test_number < argc; test_number++)
	{
		IFRUN_TEST(test_list_1);
		IFRUN_TEST(test_cycle_error);
		IFRUN_TEST(test_free_list);
	}
}
