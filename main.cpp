#include <stdio.h> 
#include <malloc.h>

#include "include/list.h"



int main()
{
	struct List list;

	list_init(&list, START_SIZE);

	list_dump(&list);


	return 0;
}