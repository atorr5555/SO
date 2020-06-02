#ifndef process_table
#define process_table
#include<stdio.h>
#include<stdlib.h>
#include"dir_table.h"


typedef struct Row_process_table {
	int num_process;
	int size;
	int num_pages;
	struct Row_process_table *next_row;
	Dir_table direction_table;
} Row_process_table;

typedef struct Process_table {
	Row_process_table *first_row;
} Process_table;

Process_table create_process_table();
void add_process(Process_table *,int, int, int);
int get_size_ptable(Process_table);
void print_ptable(Process_table);
int find_process(Process_table, int);
void print_dir_tables(Process_table);

Process_table create_process_table() {
	Process_table table;
	table.first_row = NULL;
	return table;
}

void add_process(Process_table *table, int num_process, int size, int num_pages) {
	Row_process_table *new_element = (Row_process_table *) malloc(sizeof(Row_process_table));
	new_element->num_process = num_process;
	new_element->size = size;
	new_element->num_pages = num_pages;
	Dir_table new_dir_table = create_dir_table();
	new_element->direction_table = new_dir_table;
	if (table->first_row == NULL) {
		table->first_row = new_element;
		return;
	}
	Row_process_table *current = table->first_row;
	while (current->next_row != NULL) {
    current = current->next_row;
 	}
	current->next_row = new_element;
}

int get_size_ptable (Process_table table) {
	int count = 0;
	Row_process_table *tmp = table.first_row;
	while (tmp!= NULL){
		count++;
		tmp = tmp->next_row;
	}
	return count;
}

void print_ptable(Process_table table) {
	if (table.first_row == NULL) {
		printf("La tabla está vacía");
		return;
	}
	Row_process_table *current = table.first_row;
	printf("Proceso\tTamaño\t#Paginas\n");
	while (current != NULL) {
		printf("%d\t%d\t%d \n", current->num_process, current->size, current->num_pages);
		current = current->next_row;
	}
}

int find_process(Process_table table, int num_process) {
	if (table.first_row == NULL) {
		return -1;
	}
	Row_process_table *current = table.first_row;
	for (int i = 0; i < get_size_ptable(table); i++) {
		if (current->num_process == num_process) {
			return i;
		}
	}
	return -1;	
}

void print_dir_tables(Process_table table) {
	Row_process_table *current = table.first_row;
	while(current != NULL) {
		printf("Proceso: %d\n", current->num_process);
		print_dtable(current->direction_table);
		current = current->next_row;
	}
}

#endif