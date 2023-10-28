#include <stdio.h>
#include <malloc.h>


#include "include/list.h"

err_t list_init(struct List* list_ptr, const ssize_t start_size)
{
	err_t return_code = NO_ERROR;

	list_ptr->data_size = start_size;

	list_ptr->data = (elem_t*) calloc(start_size, sizeof(elem_t));

	list_ptr->next = (elem_t*) malloc(start_size * sizeof(elem_t));

	(list_ptr->next)[0] = 0;

	list_ptr->prev = (elem_t*) malloc(start_size * sizeof(elem_t));

	(list_ptr->prev)[0] = 0;

	list_ptr->head = 0;

	list_ptr->tail = 0;

	for (ssize_t n_data_elem = 1; n_data_elem < list_ptr->data_size; n_data_elem++)
	{
		(list_ptr->next)[n_data_elem] = -1;
		(list_ptr->prev)[n_data_elem] = -1;
	}

	find_free_positions_list_with_cycle(list_ptr);

	return return_code;
}


err_t list_dtor(struct List* list_ptr)
{
	CHECK_LIST();

	free(list_ptr->data);
	free(list_ptr->next);
	free(list_ptr->prev);

	list_ptr->data = 0;
	list_ptr->next = 0;
	list_ptr->prev = 0;

	list_ptr->data_size = -1;
	list_ptr->tail = -1;
	list_ptr->head = -1;
	list_ptr->free = -1;

	return return_code;
}


err_t check_transition_arr(const ssize_t* const transition, const ssize_t start_postion, const ssize_t arr_size, enum ListErrors INVALID_TRANSITION, enum ListErrors INF_CYCLE)
{
	err_t return_code = NO_ERROR;

	ssize_t position = start_postion;

	ssize_t transition_iterations = 0;

	while(transition[position] != 0)
	{
		if ((transition[position] < 0) || transition[position] >= arr_size)
		{
			return_code |= INVALID_TRANSITION;
			return return_code;
		}

		if (transition_iterations >= arr_size)
		{
			return_code |= INF_CYCLE;
			return return_code;
		}

		position = transition[position];

		transition_iterations++;
	}

	return return_code;
}


err_t list_verificator(const struct List* const list_ptr)
{
	err_t return_code = NO_ERROR;

	if (list_ptr->data_size < 0) return_code |= INVALID_DATA_SIZE;
	if (list_ptr->data == NULL)  return_code |= INVALID_DATA_PTR;
	if (list_ptr->next == NULL)  return_code |= INVALID_NEXT_PTR;
	if (list_ptr->prev == NULL)  return_code |= INVALID_PREV_PTR;

	if ((list_ptr->free  < 0) || (list_ptr->free >= list_ptr->data_size))
	{
		return_code |= INVALID_FREE_VAL;
	}

	if ((list_ptr->head  < 0) || (list_ptr->head >= list_ptr->data_size))
	{
		return_code |= INVALID_HEAD_VAL;
	}

	if ((list_ptr->tail  < 0) || (list_ptr->tail >= list_ptr->data_size))
	{
		return_code |= INVALID_TAIL_VAL;
	}

	if ((list_ptr->prev)[0] != 0) return_code |= INVALID_FIRST_PREV_ELEM_VAL;
	if ((list_ptr->next)[0] != 0) return_code |= INVALID_FIRST_NEXT_ELEM_VAL;

	for (ssize_t n_data_elem = 1; n_data_elem < list_ptr->data_size; n_data_elem++)
	{
		if ((list_ptr->next)[n_data_elem] < -1) return_code |= INVALID_NEXT_ELEM_VAL;
		if ((list_ptr->prev)[n_data_elem] < -1) return_code |= INVALID_PREV_ELEM_VAL;
	}	

	CHECK_NEXT_ARR(list_ptr->next, list_ptr->head, list_ptr->data_size);
	CHECK_PREV_ARR(list_ptr->prev, list_ptr->tail, list_ptr->data_size);

	return return_code;
}


err_t print_list_error(const err_t return_code)
{
	if (return_code & INVALID_DATA_SIZE) fprintf(stderr, "invalid data size\n");
	if (return_code & INVALID_DATA_PTR)  fprintf(stderr, "invalid data ptr\n");
	if (return_code & INVALID_NEXT_PTR)  fprintf(stderr, "invalid next ptr\n");
	if (return_code & INVALID_PREV_PTR)  fprintf(stderr, "invalid prev ptr\n");
	if (return_code & INVALID_FREE_VAL)  fprintf(stderr, "invaid free val\n");
	if (return_code & INVALID_HEAD_VAL)  fprintf(stderr, "invalid head val\n");
	if (return_code & INVALID_TAIL_VAL)  fprintf(stderr, "invalid tail val\n");
	if (return_code & INVALID_PREV_ELEM_VAL) fprintf(stderr, "invalid prev elem val\n");
	if (return_code & INVALID_NEXT_ELEM_VAL) fprintf(stderr, "invalid next elem val\n");
	if (return_code & ERROR_NEXT_PREV) fprintf(stderr, "next from prev from value position is not equal to the original value position\n");
	if (return_code & ERROR_PREV_NEXT) fprintf(stderr, "prev from next from value position is not equal to the original value position\n");

	if (return_code & INVALID_FIRST_PREV_ELEM_VAL) fprintf(stderr, "invalid first special value in prev array\n");
	if (return_code & INVALID_FIRST_NEXT_ELEM_VAL) fprintf(stderr, "invalid first special value in next array\n");

	if (return_code & PREV_INF_CYCLE) fprintf(stderr, "looping transition to previous\n");
	if (return_code & NEXT_INF_CYCLE) fprintf(stderr, "looping transition to next\n");

	if (return_code & INVALID_PREV_TRANSITION) fprintf(stderr, "invalid previous transition\n");
	if (return_code & INVALID_NEXT_TRANSITION) fprintf(stderr, "invlaid next transition\n");

	if (return_code & WRONG_BEFORE_POSITION) fprintf(stderr, "wrong before position\n");

	return return_code;
}


err_t print_transition_array(const ssize_t* arr, const char* arr_name, const ssize_t arr_size)
{
	printf("\t%s [%p]:\n", arr_name, arr);
	printf("\t{\n");

	for (ssize_t n_arr_elem = 0; n_arr_elem < arr_size; n_arr_elem++)
	{
		printf("\t" NUMBER_PRINTF_SPEC " ", n_arr_elem);
	}

	putchar('\n');

	for (ssize_t n_arr_elem = 0; n_arr_elem < arr_size; n_arr_elem++)
	{	
		printf("\t" NUMBER_PRINTF_SPEC " ", arr[n_arr_elem]);
	}

	putchar('\n');

	printf("\t}\n");

	
	return NO_ERROR;
}


err_t print_list_data(const elem_t* data, const char* data_name, const ssize_t data_size)
{
	printf("\t%s [%p]:\n", data_name, data);

	printf("\t{\n");

	for (ssize_t n_data_elem = 0; n_data_elem < data_size; n_data_elem++)
	{
		printf("\t" NUMBER_PRINTF_SPEC " ", n_data_elem);
	}

	putchar('\n');

	for (ssize_t n_data_elem = 0; n_data_elem < data_size; n_data_elem++)
	{
		printf("\t" LIST_ELEM_PRINTF_SPEC " ", data[n_data_elem]);
	}

	putchar('\n');
	printf("\t}\n");	

	return NO_ERROR;
}


err_t list_dump(const struct List* const list_ptr)
{
	CHECK_LIST();

	printf("struct List list [%p]\n", list_ptr);
	printf("{\n");
	printf("\tdata size =  %zd\n", list_ptr->data_size);
	printf("\tfree = %zd\n", list_ptr->free);
	printf("\thead = %zd\n", list_ptr->head);
	printf("\ttail = %zd\n", list_ptr->tail);
	putchar('\n');
	
	PRINT_LIST_DATA(list_ptr->data, list_ptr->data_size);

	PRINT_TRANSITION_ARRAY(list_ptr->next, list_ptr->data_size);
	PRINT_TRANSITION_ARRAY(list_ptr->prev, list_ptr->data_size);

	printf("}\n");

	return return_code;
}


err_t find_free_positions_list_with_cycle(struct List* list_ptr)
{
	CHECK_LIST();

	ssize_t position = list_ptr->free;

	while((list_ptr->next)[position] != 0)
	{
		position = list_ptr->next[position];
	}

	for (ssize_t n_data_elem = 1; n_data_elem < list_ptr->data_size; n_data_elem++)
	{
		if ((list_ptr->next)[n_data_elem] == -1)
		{

			if (position == 0)
			{
				list_ptr->free = n_data_elem;
			}
			else
			{
				(list_ptr->next)[position] = n_data_elem;	
			}
			
			position = n_data_elem;
		}
	}

	(list_ptr->next)[position] = 0;

	return return_code;
}


err_t free_cell(struct List* list_ptr, const ssize_t position)
{
	CHECK_LIST();

	ssize_t before_position = (list_ptr->prev)[position];

	ssize_t after_position = (list_ptr->next)[position];

	if (before_position != 0)
	{
		(list_ptr->next)[before_position] = after_position;	
	}
	else
	{
		list_ptr->head = after_position;
	}

	if (after_position != 0)
	{
		(list_ptr->prev)[after_position] =  before_position;
	}
	else
	{
		list_ptr->tail = before_position;
	}

	

	(list_ptr->data)[position] = 0;

	(list_ptr->next)[position] = list_ptr->free;

	(list_ptr->prev)[position] = -1;

	list_ptr->free = position;

	return return_code;
}


static ssize_t get_free_elem_pos(struct List* list_ptr)
{
	CHECK_LIST();

	ssize_t position = list_ptr->free;
	list_ptr->free = list_ptr->next[list_ptr->free];

	return position;
}


static err_t set_tail_head_first_elem(struct List* list_ptr, const ssize_t position)
{
	CHECK_LIST();

	(list_ptr->next)[position] = 0;
	(list_ptr->prev)[position] = 0;

	list_ptr->tail = position;
	list_ptr->head = position;	

	return return_code;
}


err_t add_elem_in_head(struct List* list_ptr, const elem_t elem)
{
	CHECK_LIST();

	ssize_t position = get_free_elem_pos(list_ptr);

	(list_ptr->data)[position] = elem;	

	if (list_ptr->head == 0)
	{
		set_tail_head_first_elem(list_ptr, position);
	}
	else
	{
		(list_ptr->prev)[list_ptr->head] = position;

		(list_ptr->next)[position] = list_ptr->head;

		list_ptr->head = position;

		list_ptr->head = position;

		(list_ptr->prev)[position] = 0;
	}

	return return_code;
}


err_t add_elem_in_tail(struct List* list_ptr, const elem_t elem)
{
	CHECK_LIST();

	ssize_t position = get_free_elem_pos(list_ptr);

	(list_ptr->data)[position] = elem;

	if (list_ptr->tail == 0)
	{
		set_tail_head_first_elem(list_ptr, position);
	}
	else
	{
		(list_ptr->next)[list_ptr->tail] = position;

		(list_ptr->prev)[position] = list_ptr->tail;

		list_ptr->tail = position;

		(list_ptr->next)[position] = 0;
	}

	return return_code;
}


err_t add_elem_after_position(struct List* list_ptr, elem_t elem,  const ssize_t before_position)
{
	CHECK_LIST();

	if ((list_ptr->prev)[before_position] < 0)
	{
		return_code |= WRONG_BEFORE_POSITION;
		print_list_error(return_code);

		return return_code;
	}
	if (before_position == list_ptr->tail)
	{
		return_code |= add_elem_in_tail(list_ptr, elem);

		return return_code;
	}

	ssize_t position = get_free_elem_pos(list_ptr);

	(list_ptr->data)[position] = elem;

	const ssize_t after_position = (list_ptr->next)[before_position];

	(list_ptr->next)[before_position] = position;
	(list_ptr->prev)[position] = before_position;

	(list_ptr->prev)[after_position] = position;
	(list_ptr->next)[position]  = after_position;

	return return_code;
}