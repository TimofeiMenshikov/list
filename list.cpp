#include <stdio.h>
#include <malloc.h>

#include "include/list.h"

#define CHECK_INCREASE_LIST() if (list_ptr->free == -1) return_code |= list_increase(list_ptr);

#define CHECK_BAD_RETURN() 				\
	if (bad_return)						\
	{									\
		print_list_error(return_code);	\
		return return_code;				\
	}									

#define LIST_TAIL (list_ptr->next)[0]
#define LIST_HEAD (list_ptr->prev)[0]

err_t list_init(struct List* const list_ptr, const ssize_t start_size)
{
	err_t return_code = NO_ERROR;

	list_ptr->data_size = start_size;

	list_ptr->data = (elem_t*) calloc(start_size, sizeof(elem_t));

	list_ptr->next = (elem_t*) malloc(start_size * sizeof(elem_t));

	list_ptr->prev = (elem_t*) malloc(start_size * sizeof(elem_t));

	LIST_HEAD = 0;

	LIST_TAIL = 0;

	list_ptr->free = -1;

	list_ptr->is_able_to_decrease = -1;

	list_ptr->last_add_position = -1;
	list_ptr->last_free_position = -1;

	list_ptr->list_elem_count = 0;

	for (ssize_t n_data_elem = 1; n_data_elem < list_ptr->data_size; n_data_elem++)
	{
		(list_ptr->next)[n_data_elem] = -1;
		(list_ptr->prev)[n_data_elem] = -1;
	}

	list_dump(list_ptr);

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
	list_ptr->free = -1;

	list_ptr->last_add_position = -1;
	list_ptr->last_free_position = -1;

	list_ptr->list_elem_count = -1;
	list_ptr->is_able_to_decrease = -1;

	return return_code;
}


static err_t check_transition_arr(const ssize_t* const transition, const ssize_t* reverse_transition,  const ssize_t start_postion, const ssize_t arr_size, const enum ListErrors invalid_transition, const enum ListErrors inf_cycle)
{
	err_t return_code = NO_ERROR;

	ssize_t position = start_postion;

	ssize_t transition_iterations = 0;

	while(transition[position] != 0)
	{
		if ((transition[position] < 0) || transition[position] >= arr_size)
		{
			return_code |= invalid_transition;
			return return_code;
		}

		if (transition_iterations >= arr_size)
		{
			return_code |= inf_cycle;
			return return_code;
		}

		if (reverse_transition[transition[position]] != position)
		{
			return_code |= invalid_transition;
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

	if ((list_ptr->free  < -1) || (list_ptr->free >= list_ptr->data_size))
	{
		return_code |= INVALID_FREE_VAL;
	}

	if ((LIST_HEAD  < 0) || (LIST_HEAD >= list_ptr->data_size))
	{
		return_code |= INVALID_HEAD_VAL;
	}

	if ((LIST_TAIL  < 0) || (LIST_TAIL >= list_ptr->data_size))
	{
		return_code |= INVALID_TAIL_VAL;
	}

	if (((list_ptr->prev)[0] < 0) || ((list_ptr->prev)[0] >= list_ptr->data_size)) return_code |= INVALID_FIRST_PREV_ELEM_VAL;
	if (((list_ptr->next)[0] < 0) || ((list_ptr->next)[0] >= list_ptr->data_size)) return_code |= INVALID_FIRST_NEXT_ELEM_VAL;

	if ((list_ptr->list_elem_count >= list_ptr->data_size) || (list_ptr->list_elem_count < 0))
	{
		return_code |= INVALID_LIST_ELEM_COUNT;
	}

	for (ssize_t n_data_elem = 1; n_data_elem < list_ptr->data_size; n_data_elem++)
	{
		if ((list_ptr->next)[n_data_elem] < -1) return_code |= INVALID_NEXT_ELEM_VAL;
		if ((list_ptr->prev)[n_data_elem] < -1) return_code |= INVALID_PREV_ELEM_VAL;
	}	

	return_code |= check_transition_arr(list_ptr->next, list_ptr->prev, LIST_HEAD, list_ptr->data_size, INVALID_NEXT_TRANSITION, NEXT_INF_CYCLE);


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
	if (return_code & WRONG_AFTER_POSITION)  fprintf(stderr, "wrong after position\n");

	if (return_code & UNABLE_TO_FREE_ZERO_ELEMENT) fprintf(stderr, "unable to free zero element\n");

	if (return_code & INVALID_POSITION_TO) fprintf(stderr, "invalid position to move list element to\n");
	if (return_code & INVALID_POSITION_FROM) fprintf(stderr, "invalid position to move list element from\n");

	if (return_code & INVALID_SWAP_POSITION) fprintf(stderr, "invalid list element position to swap\n");

	if (return_code & LIST_IS_NOT_ABLE_TO_DECREASE) fprintf(stderr, "Unable to decrease list. It's not ready to realloc down\n");

	if (return_code & INVALID_LIST_ELEM_COUNT) fprintf (stderr, "invalid list element count (list.list_elem_count)\n");

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
	printf("\thead = %zd\n", LIST_HEAD);
	printf("\ttail = %zd\n", LIST_TAIL);
	printf("\tlist_elem_count = %zd\n", list_ptr->list_elem_count);
	printf("\tis able to decrease = %d\n", list_ptr->is_able_to_decrease);
	putchar('\n');
	
	PRINT_LIST_DATA(list_ptr->data, list_ptr->data_size);

	PRINT_TRANSITION_ARRAY(list_ptr->next, list_ptr->data_size);
	PRINT_TRANSITION_ARRAY(list_ptr->prev, list_ptr->data_size);

	printf("}\n");

	return return_code;
}


err_t find_free_positions_list_with_cycle(struct List* const list_ptr)
{
	CHECK_LIST();

	ssize_t new_free = -1;
	ssize_t position = -1;

	for (ssize_t n_data_elem = 1; n_data_elem < list_ptr->data_size; n_data_elem++)
	{
		if ((list_ptr->next)[n_data_elem] == -1)
		{
			if (position == -1)
			{
				new_free = n_data_elem;
			}
			else
			{
				(list_ptr->next)[position] = n_data_elem;	
			}
			
			position = n_data_elem;
		}
	}

	if (new_free != -1)
	{
		(list_ptr->next)[position] = list_ptr->free;
		list_ptr->free = new_free;
	}

	return return_code;
}


err_t update_free_positions_list_with_cycle(struct List* const list_ptr)
{
	CHECK_LIST();

	ssize_t new_free = -1;
	ssize_t position = -1;

	for (ssize_t n_data_elem = 1; n_data_elem < list_ptr->data_size; n_data_elem++)
	{
		if ((list_ptr->prev)[n_data_elem] == -1)
		{
			if (position == -1)
			{
				new_free = n_data_elem;
			}
			else
			{
				(list_ptr->next)[position] = n_data_elem;	
			}

			position = n_data_elem;
		}		
	}

	if (position != -1)
	{
		(list_ptr->next)[position] = -1;
	}
	

	list_ptr->free = new_free;

	return return_code;
}


err_t free_cell(struct List* const list_ptr, const ssize_t position)
{
	CHECK_LIST();

	if (position == 0)
	{
		return_code |= UNABLE_TO_FREE_ZERO_ELEMENT;
		print_list_error(return_code);
		return return_code;
	}

	ssize_t before_position = (list_ptr->prev)[position];

	ssize_t after_position = (list_ptr->next)[position];


	(list_ptr->next)[before_position] = after_position;	
	(list_ptr->prev)[after_position] =  before_position;

	(list_ptr->data)[position] = 0;

	(list_ptr->next)[position] = list_ptr->free;

	(list_ptr->prev)[position] = -1;

	list_ptr->free = position;

	list_ptr->last_free_position = position;

	(list_ptr->list_elem_count)--;

	return return_code;
}


static ssize_t get_free_elem_pos(struct List* const list_ptr)
{
	CHECK_LIST();

	ssize_t position = list_ptr->free;
	list_ptr->free = list_ptr->next[list_ptr->free];\

	list_ptr->is_able_to_decrease = -1;

	return position;
}


static err_t link_elem_with_neighbours(struct List* const list_ptr, const ssize_t position, const ssize_t before_position, const ssize_t after_position)
{
	(list_ptr->next)[before_position] = position;
	(list_ptr->prev)[position] = before_position;

	(list_ptr->prev)[after_position] = position;
	(list_ptr->next)[position]  = after_position;

	return NO_ERROR;	
}


err_t add_elem_after_position(struct List* const list_ptr, elem_t elem,  const ssize_t before_position)
{
	CHECK_LIST();

	CHECK_INCREASE_LIST()

	if ((list_ptr->prev)[before_position] < 0)
	{
		return_code |= WRONG_BEFORE_POSITION;
		print_list_error(return_code);

		return return_code;
	}

	const ssize_t position = get_free_elem_pos(list_ptr);

	(list_ptr->data)[position] = elem;

	const ssize_t after_position = (list_ptr->next)[before_position];

	link_elem_with_neighbours(list_ptr, position, before_position, after_position);

	list_ptr->last_add_position = position;

	(list_ptr->list_elem_count)++;

	list_ptr->is_able_to_decrease = -1;

	return return_code;
}

err_t add_elem_before_position(struct List* const list_ptr, elem_t elem, const ssize_t after_position)
{
	return add_elem_after_position(list_ptr, elem, (list_ptr->prev)[after_position]);
}

err_t add_elem_in_head(struct List* const list_ptr, const elem_t elem) 
{
	return add_elem_before_position(list_ptr, elem, LIST_HEAD);
}

err_t add_elem_in_tail(struct List* const list_ptr, const elem_t elem) 
{
	return	add_elem_after_position(list_ptr, elem, LIST_TAIL);
}


#define CHECK_BAD_REALLOC_PTRS()					\
	bool bad_return = false;						\
													\
	if ((list_ptr->next) == NULL)  					\
	{												\
		list_ptr->next = old_next_arr;				\
		return_code |= UNABLE_TO_INCREASE_LIST;		\
		bad_return = true;							\
	}												\
	if ((list_ptr->prev) == NULL)					\
	{												\
		list_ptr->prev = old_prev_arr;				\
		return_code |= UNABLE_TO_INCREASE_LIST;		\
		bad_return = true;							\
	}												\
	if ((list_ptr->data) == NULL)					\
	{												\
		list_ptr->data = old_list_data;				\
		return_code |= UNABLE_TO_INCREASE_LIST;		\
		bad_return = true;							\
	}												\
													\
	CHECK_BAD_RETURN()								\



err_t list_increase(struct List* const list_ptr)
{
	CHECK_LIST();

	ssize_t* old_next_arr = list_ptr->next;
	ssize_t* old_prev_arr = list_ptr->next;
	elem_t*  old_list_data = list_ptr->data;

	list_ptr->next = (ssize_t*) realloc(list_ptr->next, LIST_INCREASE_CONSTANT * sizeof(ssize_t) * list_ptr->data_size);
	list_ptr->prev = (ssize_t*) realloc(list_ptr->prev, LIST_INCREASE_CONSTANT * sizeof(ssize_t) * list_ptr->data_size);
	list_ptr->data = (elem_t*)  realloc(list_ptr->data, LIST_INCREASE_CONSTANT * sizeof(elem_t)  * list_ptr->data_size);  

	CHECK_BAD_REALLOC_PTRS()
	 	
	for (ssize_t n_data_elem = list_ptr->data_size; n_data_elem < 2 * list_ptr->data_size; n_data_elem++)
	{
		if (n_data_elem < 2 * list_ptr->data_size - 1)
		{
			(list_ptr->next)[n_data_elem] = n_data_elem + 1; 
		}
		else 
		{
			(list_ptr->next)[n_data_elem] = list_ptr->free;
		}
		
		(list_ptr->prev)[n_data_elem] = NEXT_PREV_POISON_VALUE;
		(list_ptr->data)[n_data_elem] = 0;
	}

	list_ptr->free = list_ptr->data_size;

	list_ptr->data_size *= LIST_INCREASE_CONSTANT;

	return return_code;	
}


err_t move_elem(struct List* const list_ptr, const ssize_t position_from, const ssize_t position_to)
{
	CHECK_LIST();

	bool bad_return = false;

	if ((position_to < 0) || (position_to >= list_ptr->data_size)) 
	{
		return_code |= INVALID_POSITION_TO;
		bad_return = true;
	}

	if ((position_from < 0) || (position_from >= list_ptr->data_size))
	{
		return_code |= INVALID_POSITION_FROM;
		bad_return = true;
	}

	if ((list_ptr->prev)[position_from] == -1)
	{
		return_code |= UNABLE_TO_MOVE_FREE_ELEMENT;
		bad_return = true;
	}

	if ((list_ptr->prev)[position_to] != -1)
	{
		return_code |= UNABLE_TO_MOVE_TO_BUSY_ELEMENT_POSITION;
		bad_return = true;
	}

	CHECK_BAD_RETURN()

	(list_ptr->next)[position_to] = (list_ptr->next)[position_from];
	(list_ptr->prev)[position_to] = (list_ptr->prev)[position_from];

	(list_ptr->next)[(list_ptr->prev)[position_from]] = position_to;
	(list_ptr->prev)[(list_ptr->next)[position_from]] = position_to;

	(list_ptr->data)[position_to] = (list_ptr->data)[position_from];

	(list_ptr->data)[position_from] = 0;
	(list_ptr->next)[position_from] = NEXT_PREV_POISON_VALUE;
	(list_ptr->prev)[position_from] = NEXT_PREV_POISON_VALUE;

	if (position_to != list_ptr->free)  
	{
		update_free_positions_list_with_cycle(list_ptr);
	}
	else 
	{
		list_ptr->free = position_from;
	}

	list_ptr->is_able_to_decrease = -1;
	
	return return_code;
}


err_t swap_two_elements(struct List* const list_ptr, const ssize_t position1, const ssize_t position2)  // нужен хотя бы один свободный элемент
{
	CHECK_LIST();

	if (position1 == position2)
	{
		return return_code;
	}

	bool bad_return = false;

	if ((position1 < 0) || (position1 >= list_ptr->data_size)) 
	{
		return_code |= INVALID_SWAP_POSITION;
		bad_return = true;
	}

	if ((position2 < 0) || (position2 >= list_ptr->data_size))
	{
		return_code |= INVALID_SWAP_POSITION;
		bad_return = true;
	}

	if ((list_ptr->free) == -1)
	{
		return_code |= NO_FREE_ELEMENTS_CANT_SWAP;
		bad_return = true;
	}

	CHECK_BAD_RETURN()

	ssize_t prev_free = list_ptr->free;

	move_elem(list_ptr, position1, list_ptr->free);

	move_elem(list_ptr, position2, position1);

	move_elem(list_ptr, prev_free, position2);

	return return_code;
}


err_t group_elements(struct List* const list_ptr)  
{
	CHECK_LIST();

	for (ssize_t position1 = 1; position1 < list_ptr->data_size; position1++)
	{
		for (ssize_t position2 = position1 + 1; position2 < list_ptr->data_size; position2++)
		{
			if (((list_ptr->prev)[position2] != -1) && ((list_ptr->prev)[position1] == -1))
			{
				move_elem(list_ptr, position2, position1);
			}
		}
	}

	list_ptr->is_able_to_decrease = -1;

	return return_code;
}


err_t check_list_able_to_decrease(struct List* const list_ptr)
{
	CHECK_LIST();

	if (list_ptr->list_elem_count + 1 > list_ptr->data_size / LIST_DECREASE_CONSTANT)
	{
		list_ptr->is_able_to_decrease = false;
		return return_code;
	}

	for (ssize_t n_data_elem = (list_ptr->data_size) / LIST_DECREASE_CONSTANT; n_data_elem < list_ptr->data_size; n_data_elem++)
	{
		if ((list_ptr->prev)[n_data_elem] != -1)
		{
			list_ptr->is_able_to_decrease = false;
			return return_code;
		}
	}

	list_ptr->is_able_to_decrease = true;

	return return_code;
}


#define CHECK_ABLE_TO_DECREASE_PARAMETER()		\
if (list_ptr->is_able_to_decrease == false)		\
{												\
	return_code |= LIST_IS_NOT_ABLE_TO_DECREASE;\
	return return_code;							\
}


err_t list_decrease(struct List* const list_ptr)
{
	CHECK_LIST();

	CHECK_ABLE_TO_DECREASE_PARAMETER()

	if (list_ptr->is_able_to_decrease == -1)
	{
		return_code |= check_list_able_to_decrease(list_ptr);

		CHECK_ABLE_TO_DECREASE_PARAMETER()
	}

	ssize_t* old_next_arr = list_ptr->next;
	ssize_t* old_prev_arr = list_ptr->next;
	elem_t*  old_list_data = list_ptr->data;

	list_ptr->next = (ssize_t*) realloc(list_ptr->next,  sizeof(ssize_t) * (list_ptr->data_size / LIST_DECREASE_CONSTANT));
	list_ptr->prev = (ssize_t*) realloc(list_ptr->prev,  sizeof(ssize_t) * (list_ptr->data_size / LIST_DECREASE_CONSTANT));
	list_ptr->data = (elem_t*)  realloc(list_ptr->data,  sizeof(elem_t)  * (list_ptr->data_size / LIST_DECREASE_CONSTANT));  

	CHECK_BAD_REALLOC_PTRS()

	list_ptr->free = -1;

	list_ptr->data_size /= LIST_DECREASE_CONSTANT;

	update_free_positions_list_with_cycle(list_ptr);

	list_ptr->is_able_to_decrease = -1;

	return return_code;
}


err_t straighten_list_transitions(struct List* const list_ptr)
{
	CHECK_LIST();

	ssize_t arr_position = 1;

	ssize_t list_position = LIST_HEAD;

	for (arr_position = 1; arr_position < list_ptr->data_size; arr_position++)
	{
		if (list_position == 0)
		{
			break;
		}

		ssize_t next_list_position = (list_ptr->next)[list_position];

		if ((list_ptr->prev)[arr_position] == -1)
		{
			move_elem(list_ptr, list_position, arr_position);	
		}
		else 
		{
			swap_two_elements(list_ptr, list_position, arr_position);
		}

		list_position = next_list_position;
	}

	return return_code;
}