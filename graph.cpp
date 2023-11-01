#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include "include/list.h"
#include "include/graph.h"
#include <stdlib.h>



FILE* open_file(const char* const filename, const char* const modificator)
{
	FILE* inputfile = fopen(filename, modificator);
	assert(inputfile);
	return inputfile;
}


err_t create_one_graph(struct List* list_ptr, ssize_t* graph_number_ptr, char *  graph_filepath, const char * const dot_filepath, const char* const graph_file_extension, FILE* html_file)	
{	
	err_t return_code = list_verificator(list_ptr);

	sprintf(graph_filepath, "graph/graph%zd.png", *graph_number_ptr);						
																						
	write_to_dot_file(list_ptr, dot_filepath);											
																						
																						
																						
	RUN_DOT_FILE(graph_filepath, dot_filepath, graph_file_extension)					
																		
	CREATE_HTML_LINE("<section>")				
	fprintf(html_file, "<img src=graph%zd.png alt=\"struct List list [%p]\"/>\n", *graph_number_ptr, list_ptr);	
	CREATE_HTML_LINE("</section>")					
																						
	(*graph_number_ptr)++;	

	return return_code;
}		


err_t write_to_dot_file(struct List* list_ptr, const char* const dot_filepath)
{
	CHECK_LIST();

	FILE* dot_file = open_file(dot_filepath, "w"); 
	assert(dot_file);

	CREATE_DOT_LINE("digraph G {")
	fprintf(dot_file, "rankdir = \"LR\";\n");
	CREATE_DOT_LINE("subgraph cluster0 {")

	fprintf(dot_file, "label = \"struct List list [%p]\";", list_ptr);

	fprintf(dot_file, "fontname=\"Helvetica,Arial,sans-serif\";\n");
	fprintf(dot_file, "node [fontname=\"Helvetica,Arial,sans-serif\" fontsize = \"16\" ];\n");
	fprintf(dot_file, "edge [fontname=\"Helvetica,Arial,sans-serif\"];\n");

	CREATE_DOT_LINE("subgraph cluster1 {")
	fprintf(dot_file, "rankdir = \"TB\";\n");
	fprintf(dot_file, "label = \"list variables\"");

	fprintf(dot_file, "free [shape=box3d, label=\"free = %zd\"];\n", list_ptr->free);
	fprintf(dot_file, "head [shape=box3d, label=\"head = %zd\"];\n", list_ptr->head);
	fprintf(dot_file, "tail [shape=box3d, label=\"tail = %zd\"];\n", list_ptr->tail);


	CREATE_DOT_LINE("}")	
	CREATE_DOT_LINE("subgraph cluster2 {")
	fprintf(dot_file, "color = \"#AA5533\"");
	fprintf(dot_file, "label = \"list_ptr->data [%p] list_ptr->prev [%p] list_ptr->next [%p]\";", list_ptr->data, list_ptr->prev, list_ptr->next);

	for (ssize_t n_data_elem = 0; n_data_elem < list_ptr->data_size; n_data_elem++)
	{
		fprintf(dot_file, "\"node%zd\" [ label = \" <f1> number = " LIST_ELEM_PRINTF_SPEC " | {<f0> prev = %zd |  <f2> next  = %zd }\" shape = \"Mrecord\", color=\"Pink\"];\n", n_data_elem,   (list_ptr->data)[n_data_elem], (list_ptr->prev)[n_data_elem], (list_ptr->next)[n_data_elem]);
	}

	for (ssize_t n_data_elem = 0; n_data_elem < list_ptr->data_size - 1; n_data_elem++)
	{		
		fprintf(dot_file, "\"node%zd\":f1 -> \"node%zd\":f1 [id = %zd, color=\"White\", width = 1000];\n", n_data_elem, n_data_elem + 1,2 *  n_data_elem);		// невидимая стрелка связь блоков списка
		fprintf(dot_file, "\"node%zd\":f2 -> \"node%zd\":f0 [id = %zd, color=\"White\", width = 1000];\n", n_data_elem, n_data_elem + 1, 2 * n_data_elem + 1);		// невидимая стрелка связь блоков списка		
	}

	for (ssize_t n_data_elem = 0; n_data_elem < list_ptr->data_size; n_data_elem++)
	{
		if ((list_ptr->next)[n_data_elem] >= 0)
		{

			if ((list_ptr->prev)[n_data_elem] == -1)    fprintf(dot_file, "\"node%zd\":f2 -> \"node%zd\":f1 [id = %zd, color=\"#%06X\", style=\"dashed\"];\n", n_data_elem, (list_ptr->next)[n_data_elem], 2 * list_ptr->data_size + n_data_elem, rand() % MAX_COLOR_VALUE);
			else 										fprintf(dot_file, "\"node%zd\":f2 -> \"node%zd\":f1 [id = %zd, color=\"#%06X\"];\n", n_data_elem, (list_ptr->next)[n_data_elem], 2 * list_ptr->data_size + n_data_elem, rand() % MAX_COLOR_VALUE);

		}		
	}

	for (ssize_t n_data_elem = 0; n_data_elem < list_ptr->data_size; n_data_elem++)
	{
		if ((list_ptr->prev)[n_data_elem] >= 0)
		{
			fprintf(dot_file, "\"node%zd\":f0 -> \"node%zd\":f1 [id = %zd, color=\"#%06X\"];\n", n_data_elem, (list_ptr->prev)[n_data_elem], 3 * list_ptr->data_size + n_data_elem, rand() % MAX_COLOR_VALUE);
		}		
	}	

	CREATE_DOT_LINE("} } }")

	fclose(dot_file);


	return return_code;
}










