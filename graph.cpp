#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include "include/list.h"
#include "include/graph.h"
#include <stdlib.h>

#define CREATE_DOT_LINE(command_string) fprintf(dot_file, "" command_string "\n");
#define CREATE_HTML_LINE(command_string) fprintf(html_file, "" command_string "\n");

#define RUN_DOT_FILE(graph_filename, dot_filename, graph_file_extension)  								\
{																										\
	char system_command[MAX_COMMANDNAME_SIZE] = {};														\
	sprintf(system_command, "dot -T%s %s -o %s", graph_file_extension, dot_filename, graph_filename);	\
	system(system_command);																				\
}																										\


#define RUN_HTML_FILE(graph_folder, html_filename)												\
{																								\
	char system_command[MAX_COMMANDNAME_SIZE] = {};												\
	sprintf(system_command, "cd %s && %s", graph_folder, html_filename);						\
	printf("%s", system_command);																\
	system(system_command);																		\
}																								\

FILE* open_file(const char* const filename, const char* const modificator)
{
	FILE* inputfile = fopen(filename, modificator);
	assert(inputfile);
	return inputfile;
}


err_t create_one_graph(struct List* list_ptr, ssize_t* graph_number_ptr, char *  graph_filepath,  FILE* html_file)	
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
	fprintf(dot_file, "graph [splines=ortho];\n");
	CREATE_DOT_LINE("subgraph cluster0 {")

	fprintf(dot_file, "label = \"struct List list [%p]\";", list_ptr);

	fprintf(dot_file, "fontname=\"Helvetica,Arial,sans-serif\";\n");
	fprintf(dot_file, "node [fontname=\"Helvetica,Arial,sans-serif\" fontsize = \"16\" ];\n");
	fprintf(dot_file, "edge [fontname=\"Helvetica,Arial,sans-serif\"];\n");

	CREATE_DOT_LINE("subgraph cluster1 {")
	fprintf(dot_file, "rankdir = \"TB\";\n");
	fprintf(dot_file, "label = \"list variables\"");

	fprintf(dot_file, "data_size [shape=\"Mrecord\", label =\"data_size = %zd\"]\n", list_ptr->data_size);
	fprintf(dot_file, "free [shape=box3d, label=\"free = %zd\"];\n", list_ptr->free);
	fprintf(dot_file, "head [shape=box3d, label=\"head = %zd\"];\n", LIST_HEAD);
	fprintf(dot_file, "tail [shape=box3d, label=\"tail = %zd\"];\n", LIST_TAIL);

	fprintf(dot_file, "list_elem_count [shape=box3d, label=\"list elem count = %zd\"];\n", list_ptr->list_elem_count);

	////////////////////////////////////////////////////////
	fprintf(dot_file, "is_able_to_decrease[shape= box3d, label = \"is able to decrease = ");

	if (list_ptr->is_able_to_decrease == false) 		fprintf(dot_file, "false");
	else if (list_ptr->is_able_to_decrease == false)	fprintf(dot_file, "true");
	else 												fprintf(dot_file, "not verified");

	fprintf(dot_file, "\"];\n");

	//////////////////////////////////////////////////////
	fprintf(dot_file, "last_add_position[shape=box3d, label=\"last add position = %zd\"];\n", list_ptr->last_add_position);
	fprintf(dot_file, "last_free_position [shape=box3d, label=\"last free position = %zd\"];\n", list_ptr->last_free_position);


	CREATE_DOT_LINE("}")	
	CREATE_DOT_LINE("subgraph cluster2 {")
	fprintf(dot_file, "color = \"#AA5533\"");
	fprintf(dot_file, "label = \"list_ptr->data [%p] list_ptr->prev [%p] list_ptr->next [%p]\";", list_ptr->data, list_ptr->prev, list_ptr->next);

	for (ssize_t n_data_elem = 0; n_data_elem < list_ptr->data_size; n_data_elem++)
	{
		if      (n_data_elem == 0)                 fprintf(dot_file, "\"node%zd\" [ label = \" <f1>  number = " LIST_ELEM_PRINTF_SPEC "\\n\\n | {<f0>  prev = %zd \\n\\n | <f4> data_index = %zd \\n\\n | <f2> next  = %zd \\n\\n }\" shape = \"Mrecord\", color=\"Yellow\"];\n", n_data_elem,   (list_ptr->data)[n_data_elem], (list_ptr->prev)[n_data_elem], n_data_elem, (list_ptr->next)[n_data_elem]);  
		else if ((list_ptr->prev)[n_data_elem] == -1) fprintf(dot_file, "\"node%zd\" [ label = \" <f1> number = " LIST_ELEM_PRINTF_SPEC "\\n\\n | {<f0>  prev = %zd \\n\\n | <f4> data_index = %zd \\n\\n | <f2> next  = %zd \\n\\n }\" shape = \"Mrecord\", color=\"Green\"];\n", n_data_elem,   (list_ptr->data)[n_data_elem], (list_ptr->prev)[n_data_elem], n_data_elem, (list_ptr->next)[n_data_elem]);
		else                                     fprintf(dot_file, "\"node%zd\" [ label = \" <f1> number = " LIST_ELEM_PRINTF_SPEC "\\n\\n | {<f0>  prev = %zd \\n\\n | <f4> data_index = %zd \\n\\n | <f2> next  = %zd \\n\\n }\" shape = \"Mrecord\", color=\"Red\"];\n", n_data_elem,   (list_ptr->data)[n_data_elem], (list_ptr->prev)[n_data_elem], n_data_elem, (list_ptr->next)[n_data_elem]); 
	}


	/////////////////////////////////////////////////////////////////////////
	fprintf(dot_file, "node0");

	for (ssize_t n_data_elem = 1; n_data_elem < list_ptr->data_size; n_data_elem++)
	{			
		fprintf(dot_file, "->node%zd", n_data_elem);	// невидимая стрелка связь блоков списка
	}

	fprintf(dot_file, "[weight = 10000, color = \"White\"]\n");
	//////////////////////////////////////////////////////////////////////////

	for (ssize_t n_data_elem = 0; n_data_elem < list_ptr->data_size; n_data_elem++)
	{
		if ((list_ptr->next)[n_data_elem] >= 0)
		{

			if ((list_ptr->prev)[n_data_elem] == -1)    fprintf(dot_file, "\"node%zd\":f2 -> \"node%zd\":f0 [id = %zd, color=\"#%06llX\", style=\"dashed\", constraint = false, splines = polyline];\n", n_data_elem, (list_ptr->next)[n_data_elem], 2 * list_ptr->data_size + n_data_elem, rand() % MAX_COLOR_VALUE);
			else 										
			{
				if ((list_ptr->next)[(list_ptr->prev)[n_data_elem]] == n_data_elem)
				{
					fprintf(dot_file, "\"node%zd\":f2 -> \"node%zd\":f0 [id = %zd, color=\"#%06llX\", constraint = false, splines = polyline];\n", n_data_elem, (list_ptr->next)[n_data_elem], 2 * list_ptr->data_size + n_data_elem, rand() % MAX_COLOR_VALUE);
				}
				else 
				{
					fprintf(dot_file, "\"node%zd\":f2 -> \"node%zd\":f0 [id = %zd, color=\"Red\", constraint = false, splines = polyline];\n", n_data_elem, (list_ptr->next)[n_data_elem], 2 * list_ptr->data_size + n_data_elem);
					fprintf(dot_file, "\"node%zd\":f0 -> \"node%zd\":f2 [color=\"Red\", constraint = false, splines = polyline];\n", (list_ptr->next)[n_data_elem], (list_ptr->prev)[(list_ptr->next)[n_data_elem]]);

				}
			}
		}		
	}

	/*for (ssize_t n_data_elem = 0; n_data_elem < list_ptr->data_size; n_data_elem++)
	{
		if ((list_ptr->prev)[n_data_elem] >= 0)
		{
			fprintf(dot_file, "\"node%zd\":f0 -> \"node%zd\":f2 [id = %zd, color=\"#%06X\", constraint = false, splines = none];\n", n_data_elem, (list_ptr->prev)[n_data_elem], 3 * list_ptr->data_size + n_data_elem, rand() % MAX_COLOR_VALUE);
		}		
	}	*/


	//fprintf(dot_file, " free -> \"node%zd\": f4 [color = \"#%06X\"]", (list_ptr->free), rand() % MAX_COLOR_VALUE);

	CREATE_DOT_LINE("} } }")

	fclose(dot_file);


	return return_code;
}










