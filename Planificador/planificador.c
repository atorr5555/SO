#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "queue.h"
#include "process_table.h"
#include "dir_table.h"

int read_file();
void print_memoria();

/* 
tabla de procesos 
(cada registro de proceso en la tabla tiene asociada una tabla de direcciones)
Veáse process_table.h
*/
Process_table proc_table;
int memoria[5][3];

void main() {
	proc_table = create_process_table();
	if (read_file() == 0) {
		return;
	}
	print_ptable(proc_table);
	print_dir_tables(proc_table);
	//Llenando la memoria real
	Row_process_table *current = proc_table.first_row;
	int num_pass = 0;
	int i = 0;
	while (current != NULL && i < 5) {
		if ((num_pass + 1) > current->num_pages) {
			current = current->next_row;
			continue;
		}
		memoria[i][0] = current->num_process;
		memoria[i][1] = num_pass;
		memoria[i][2] = 0;
		i++;
		current = current->next_row;
		if (current == NULL && i < 5) {
			num_pass++;
			current = proc_table.first_row;
		}
	}
	print_memoria();
	
	
}

int read_file() {
	FILE *fp = fopen("prueba.txt", "r");
	if (fp == NULL){
		printf("Archivo no encontrado \n");
		return 0;
	}

	char line[64];
	char *data;
	int data_array[3];
	int num_process;
	//Lee linea por linea
	while (fgets(line, sizeof(line), fp) != NULL) {
		char *pos;
		// quita el salto de línea del final y lo sustituye por carácter '\0'
		if ((pos=strchr(line, '\n')) != NULL)
			*pos = '\0';
		data = strtok(line, " ");
		// separa por espacios
		int i = 0;
		while (data != NULL){
			data_array[i] = atoi(data);
			data = strtok(NULL, " ");// avanca a la siguiente palabra después del espacio
			i++;
		}
		// Si el primer elemento es 0 -> Es un registro maestro
		if (data_array[0] == 0) {
			num_process = data_array[1];
			double size_double = data_array[2];
			int pages = ceil(size_double/(double)20);
			add_process(&proc_table, data_array[1], data_array[2], pages);
			continue;
		}
		// Si el primer elemento no es 0 -> Es un registro de dirección
		Row_process_table *current = proc_table.first_row;
		while (current != NULL) {
			if (current->num_process == num_process) {
				break;
			}
			current = current->next_row;
		}
		if (current == NULL) {
			return -1;
		}
		// Agrega direcciones a la tabla de direcciones de los procesos
		add_dir(&(current->direction_table), data_array[1], data_array[2]);
	}
	fclose(fp);
	return 1;
}

void print_memoria() {
	printf("Memoria Real\n");
	printf("Marco\tProceso\tPagina\tFrecuencia\n");
	for (int i = 0; i < 5; i++) {
		printf("%d\t", i);
		for (int j = 0; j < 3; j++) {
			printf("%d\t", memoria[i][j]);
		}
		printf("\n");
	}
	
}

