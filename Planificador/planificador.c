#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "queue.h"
#include "process_table.h"
#include "dir_table.h"
#define quantum 4

int read_file();
void print_memoria();
int find_in_mem (int, int);
void planificador();
int	check_page(int, int);

/* 
tabla de procesos 
(cada registro de proceso en la tabla tiene asociada una tabla de direcciones)
Veáse process_table.h
*/
Process_table proc_table;
int memoria[5][3];
Queue cola_procesos;

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
	//Llenando la cola de forma inicial
	cola_procesos = create_queue();
	current = proc_table.first_row;
	while (current != NULL) {
		enqueue(&cola_procesos, current->num_process);
		current = current->next_row;
	}
	print_queue(cola_procesos);
	getchar();
	planificador();
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
	printf("-------------------------------------------\n");
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

int	check_page(int num_process, int page) {
	Row_process_table *current = proc_table.first_row;
	while (current != NULL) {
		if (current->num_process == num_process) {
			if ((page + 1) > current->num_pages) {
				return 0;
			}
			return 1;
		}
		current = current->next_row;
	}
	return 0;
}

int find_in_mem (int num_process, int page) {
	for (int i = 0; i < 5; i++) {
		if (memoria[i][0] == num_process && memoria[i][1] == page) {
			return i;
		}
	}
	return -1;
	
}

void planificador() {
	int num_process = dequeue(&cola_procesos);
	Dir_table *tabla_direcciones = NULL;
	int num_pag;
	int offset;
	int marco_pag;
	int count = 0;
	int dir_real;
	printf("-------------------------------------------\n");
	printf("Proceso %d entra a ejecucion\n", num_process);
	while (num_process != -1) {
		// Obtener su tabla de direcciones
		Row_process_table *current = proc_table.first_row;
		while (current != NULL) {
			if (current->num_process == num_process) {
				tabla_direcciones = &(current->direction_table);
			}
			current = current->next_row;
		}
		// Si no hay direcciones para traducir
		if (tabla_direcciones->first_row == NULL) {
			num_process = dequeue(&cola_procesos);
			continue;
		}
		// Obteniendo la direccion virtual de la tabla de direcciones
		num_pag = tabla_direcciones->first_row->page;
		offset = tabla_direcciones->first_row->offset;
		// Revisa desbordamiento de pagina
		if (offset >= 20) {
			printf("-------------------------------------------\n");
			printf("DESBORDAMIENTO DE PAGINA PROCESO: %d\n", num_process);
			printf("Causado por la direccion virtual: (%d, %d)\n", num_pag, offset);
			printf("El proceso %d ha terminado\n", num_process);
			num_process = dequeue(&cola_procesos);
			printf("-------------------------------------------\n");
			printf("Proceso %d entra a ejecucion\n", num_process);
			continue;
		}
		// Revisa inexistencia de pagina
		if (!check_page(num_process, num_pag)) {
			printf("-------------------------------------------\n");
			printf("INEXISTENCIA DE PAGINA PROCESO: %d\n", num_process);
			printf("Causado por la direccion virtual: (%d, %d)\n", num_pag, offset);
			printf("El proceso %d ha terminado\n", num_process);
			num_process = dequeue(&cola_procesos);
			printf("-------------------------------------------\n");
			printf("Proceso %d entra a ejecucion\n", num_process);
			continue;
		}
		// Revisa fallo de pagina
		marco_pag = find_in_mem(num_process, num_pag);
		if (marco_pag == -1) {
			printf("-------------------------------------------\n");
			printf("FALLO DE PAGINA PROCESO: %d\n", num_process);
			printf("Causado por la direccion virtual: (%d, %d)\n", num_pag, offset);
			// Llamar a funcion que maneje los cambios de página
			exit(0);
		}
		// Traduce direccion virtual a direccion real
		dir_real = marco_pag*20 + offset;
		print_memoria();
		printf("Direccion virtual: (%d, %d) | Direccion real: %d\n", num_pag, offset, dir_real);
		delete_head(tabla_direcciones);
		//Llamar a funcion que aumente la frecuencia
		count++;
		if (count >= quantum) {
			// Si todavía no termina el proceso, vuelve a la cola
			if (tabla_direcciones->first_row != NULL) {
				enqueue(&cola_procesos, num_process);
			}
			num_process = dequeue(&cola_procesos);
			printf("-------------------------------------------\n");
			printf("Proceso %d entra a ejecucion\n", num_process);
			count = 0;
		}
	}
}