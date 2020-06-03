#ifndef dir_table
#define dir_table
#include<stdio.h>
#include<stdlib.h>

typedef struct Row_dir_table {
	int page;
	int offset;
	struct Row_dir_table *next_row;
}Row_dir_table;

typedef struct Dir_table {
	int num_process;
	Row_dir_table *first_row;
}	Dir_table;

Dir_table create_dir_table();
void add_dir(Dir_table *, int, int);
int get_size_dtable(Dir_table);
void print_dtable(Dir_table);
void delete_head(Dir_table *);

Dir_table create_dir_table() {
	Dir_table table;
	table.first_row = NULL;
	return table;
}

void add_dir(Dir_table *table, int page, int offset) {
	Row_dir_table *new_element = (Row_dir_table *) malloc(sizeof(Row_dir_table));
	new_element->page = page;
	new_element->offset = offset;
	new_element->next_row = NULL;
	if (table->first_row == NULL) {
		table->first_row = new_element;
		return;
	}
	Row_dir_table *current = table->first_row;
	while (current->next_row != NULL) {
    current = current->next_row;
 	}
	current->next_row = new_element;
}

int get_size_dtable (Dir_table table) {
	int count = 0;
	Row_dir_table *tmp = table.first_row;
	while (tmp != NULL){
		count++;
		tmp = tmp->next_row;
	}
	return count;
}

void print_dtable(Dir_table table) {
	if (table.first_row == NULL) {
		printf("La tabla está vacía");
		return;
	}
	Row_dir_table *current = table.first_row;
	printf("Pagina\tDesplazamiento\n");
	while (current != NULL) {
		printf("%d\t%d\n", current->page, current->offset);
		current = current->next_row;
	}
}

void delete_head(Dir_table *table) {
	if (table->first_row == NULL) {
		return;
	}
	Row_dir_table *new_head = table->first_row->next_row;
	free(table->first_row);
	table->first_row = new_head;
}
#endif