#ifndef DOT_H
	#define DOT_H

	err_t write_to_dot_file(struct List* list_ptr, const char* const dot_filename);

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

	
	

	FILE* open_file(const char* const filename, const char* const modificator);

	err_t create_one_graph(struct List* list_ptr, ssize_t* graph_number_ptr, char *  graph_filepath, const char * const dot_filepath, const char* const graph_file_extension, FILE*  html_file);

	#define CREATE_ONE_GRAPH() create_one_graph(&list, &number, graph_filepath, dot_filepath, graph_file_extension, html_file);	
	
															

	const ssize_t MAX_FILENAME_SIZE = 1000;
	const ssize_t MAX_COMMANDNAME_SIZE = 1000;

	const ssize_t MAX_COLOR_VALUE = 16777215;

	const char html_filename[MAX_FILENAME_SIZE] = "graph.html"; 
	const char dot_filepath[MAX_FILENAME_SIZE] = "graph/test.dot";
	const char html_filepath[MAX_FILENAME_SIZE] = "graph/graph.html"; 
	const char graph_file_extension[MAX_FILENAME_SIZE] = "png";
	const char graph_folder[MAX_FILENAME_SIZE] = "graph";

#endif /* DOT_H */
