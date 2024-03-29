#ifndef LIST_H
	#define LIST_H

	const ssize_t LIST_INCREASE_CONSTANT = 2; 
	const ssize_t LIST_DECREASE_CONSTANT = 2;
	const ssize_t START_SIZE = 5;

	typedef unsigned int err_t;
	typedef ssize_t elem_t;

	#define NUMBER_PRINTF_SPEC     "%6zd"
	#define LIST_ELEM_PRINTF_SPEC  "%6zd"

	#define LIST_TAIL (list_ptr->next)[0]
	#define LIST_HEAD (list_ptr->prev)[0]

	const ssize_t NEXT_PREV_POISON_VALUE = -1;

	#define CHECK_LIST()															\
	err_t return_code = list_verificator(list_ptr); 								\
	if (return_code != NO_ERROR)													\
	{																				\
		printf("invalid list\n");													\
		printf("file: <%s> func: <%s> line <%d>\n", __FILE__, __func__, __LINE__);  \
		print_list_error(return_code);												\
	}																				\

	#define PRINT_TRANSITION_ARRAY(arr, arr_size) print_transition_array(arr, #arr, arr_size)

	#define PRINT_LIST_DATA(data, data_size) print_list_data(data, #data, data_size)

	#define CHECK_NEXT_ARR(next, start_position, arr_size) return_code |= check_transition_arr(next, start_position, arr_size, INVALID_NEXT_TRANSITION, NEXT_INF_CYCLE)
	#define CHECK_PREV_ARR(prev, start_position, arr_size) return_code |= check_transition_arr(prev, start_position, arr_size, INVALID_PREV_TRANSITION, PREV_INF_CYCLE)

	struct List
	{
		ssize_t free;
		
		ssize_t last_add_position;
		ssize_t last_free_position;

		ssize_t data_size;

		elem_t* data;
		ssize_t* next;
		ssize_t* prev;

		ssize_t list_elem_count;

		int is_able_to_decrease;
	};

	enum ListErrors
	{
		NO_ERROR 				= 0,
		INVALID_DATA_SIZE 		= 1 << 0,
		INVALID_DATA_PTR  		= 1 << 1,
		INVALID_NEXT_PTR  		= 1 << 2,
		INVALID_PREV_PTR  		= 1 << 3,

		INVALID_FREE_VAL  		= 1 << 4,
		INVALID_HEAD_VAL  		= 1 << 5,
		INVALID_TAIL_VAL  		= 1 << 6,

		INVALID_PREV_ELEM_VAL 	= 1 << 7,
		INVALID_NEXT_ELEM_VAL   = 1 << 8,

		ERROR_NEXT_PREV 		= 1 << 9,
		ERROR_PREV_NEXT   		= 1 << 10,

		INVALID_FIRST_PREV_ELEM_VAL = 1 << 11,
		INVALID_FIRST_NEXT_ELEM_VAL = 1 << 11,

		PREV_INF_CYCLE = 1 << 12,
		NEXT_INF_CYCLE = 1 << 13,

		INVALID_PREV_TRANSITION = 1 << 14,
		INVALID_NEXT_TRANSITION = 1 << 15,
		INVALID_LIST_ELEM_COUNT = 1 << 16,
		UNABLE_TO_INCREASE_LIST = 1 << 17
	};

	enum ListExeptions
	{
		WRONG_BEFORE_POSITION   = 1 << 18,
		WRONG_AFTER_POSITION    = 1 << 19,
		UNABLE_TO_FREE_ZERO_ELEMENT = 1 << 20,
		UNABLE_TO_MOVE_FREE_ELEMENT = 1 << 21,
		UNABLE_TO_MOVE_TO_BUSY_ELEMENT_POSITION = 1 << 22,
		INVALID_POSITION_TO   					= 1 << 23,           
		INVALID_POSITION_FROM 					= 1 << 24,
		INVALID_SWAP_POSITION 					= 1 << 25,
		NO_FREE_ELEMENTS_CANT_SWAP 				= 1 << 26,
		LIST_IS_NOT_ABLE_TO_DECREASE 			= 1 << 27
	};

	err_t list_init(struct List* list_ptr, const ssize_t start_size);
	err_t list_dtor(struct List* list_ptr);
	err_t list_verificator(const struct List* const list_ptr);
	err_t print_list_error(const err_t return_code);

	err_t print_transition_array(const ssize_t* arr, const char* arr_name, const ssize_t arr_size);
	err_t list_dump(const struct List* const list_ptr);

	err_t find_free_positions_list_with_cycle(struct List* list_ptr);

	err_t free_cell(struct List* list_ptr, ssize_t position);

	err_t add_elem_after_position(struct List* list_ptr, elem_t elem,  ssize_t position);

	err_t add_elem_before_position(struct List* list_ptr, elem_t elem, const ssize_t after_position);

	err_t add_elem_in_tail(struct List* list_ptr, const elem_t elem);

	err_t add_elem_in_head(struct List* list_ptr, const elem_t elem);

	err_t print_list_data(const elem_t* data, const char* data_name, const ssize_t data_size);

	err_t list_increase(struct List* const list_ptr);

	err_t move_elem(struct List* const list_ptr, const ssize_t position_from, const ssize_t position_to);

	err_t swap_two_elements(struct List* const list_ptr, const ssize_t position1, const ssize_t position2);

	err_t straighten_list_transitions(struct List* const list_ptr);

	err_t group_elements(struct List* const list_ptr);

	err_t check_list_able_to_decrease(struct List* const list_ptr);

	err_t list_decrease(struct List* const list_ptr);

#endif /* LIST_H */