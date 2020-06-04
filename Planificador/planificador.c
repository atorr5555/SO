/*
Sistemas Operativos
Programa: Administración de memoria y Planificación de procesos
Integrantes:
Flores Fuentes Kevin
Torres Verástegui José Antonio

Objetivo del programa:
Elaborar un programa en Ansi C que realice la planificación de procesos por la técnica de round robin y la administración de memoria por el método de paginación por demanda y la de reemplazo de la menos frecuentemente utilizada.

El siguiente programa recibe como parametro de entrada la dirección del archivo de texto donde se encuentren los datos de los procesos,
al igual que al compilar se necesita incluir -lm puesto que se usa la libreria math.h
Ejemplo:
gcc -o proyecto planificador.c -lm
./proyecto /home/kvriboh/Escritorio/Planificador/prueba.txt

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "queue.h"  //Libreria que cuenta con las funciones y estructuras necesarias para la cola vease con más detalle queue.h
#include "process_table.h" //Libreria que cuenta con las funciones y estructuras necesarias para la tabla de procesos vease process_table.h
#include "dir_table.h" //Libreria que cuenta con las funciones y estructuras necesarias para la tabla de direcciones vease dir_table.h
#define quantum 4

int read_file(char *);
void print_memoria();
int find_in_mem(int, int);
void planificador();
int check_page(int, int);
void mod_frec(int);
int cambia_pag(int, int);
void reset_frecuencias();
void borra_pags(int);
void print_ejecucion(int);
void termina_proceso(int);
int revisa_desbordamiento(int, int, int);
void proceso_inexist(int, int, int);
int fallo_pagina(int, int, int);

/* 
tabla de procesos 
(cada registro de proceso en la tabla tiene asociada una tabla de direcciones)
Veáse process_table.h
*/
Process_table proc_table;
int memoria[5][3];
Queue cola_procesos;

void main(int argc, char *argv[])
{
	proc_table = create_process_table();
	if (read_file(argv[1]) == 0) // Leyendo el archivo con los datos
	{
		return;
	}
	print_ptable(proc_table);
	print_dir_tables(proc_table);
	//Llenando la memoria real
	Row_process_table *current = proc_table.first_row;
	int num_pass = 0;
	int i = 0;
	while (current != NULL && i < 5)
	{
		if ((num_pass + 1) > current->num_pages)
		{
			current = current->next_row;
			continue;
		}
		memoria[i][0] = current->num_process;
		memoria[i][1] = num_pass;
		memoria[i][2] = 0;
		i++;
		current = current->next_row;
		if (current == NULL && i < 5)
		{
			num_pass++;
			current = proc_table.first_row;
		}
	}
	print_memoria();
	//Llenando la cola de forma inicial
	cola_procesos = create_queue();
	current = proc_table.first_row;
	while (current != NULL)
	{
		enqueue(&cola_procesos, current->num_process);
		current = current->next_row;
	}
	print_queue(cola_procesos);
	planificador();
}

int read_file(char *filename) //Función que se encarga de leer el archivo y sacas la información de los procesos y sus direcciones
{
	FILE *fp = fopen(filename, "r");
	if (fp == NULL)
	{
		printf("Archivo no encontrado \n");
		return 0;
	}
	char line[64];
	char *data;
	int data_array[3];
	int num_process;
	//Lee linea por linea
	while (fgets(line, sizeof(line), fp) != NULL)
	{
		char *pos;
		// quita el salto de línea del final y lo sustituye por carácter '\0'
		if ((pos = strchr(line, '\n')) != NULL)
			*pos = '\0';
		data = strtok(line, " ");
		// separa por espacios
		int i = 0;
		while (data != NULL)
		{
			data_array[i] = atoi(data);
			data = strtok(NULL, " "); // avanza a la siguiente palabra después del espacio
			i++;
		}
		// Si el primer elemento es 0 -> Es un registro maestro
		if (data_array[0] == 0)
		{
			num_process = data_array[1];
			double size_double = data_array[2];
			int pages = ceil(size_double / (double)20);
			add_process(&proc_table, data_array[1], data_array[2], pages);
			continue;
		}
		// Si el primer elemento no es 0 -> Es un registro de dirección
		Row_process_table *current = proc_table.first_row;
		while (current != NULL)
		{
			if (current->num_process == num_process)
			{
				break;
			}
			current = current->next_row;
		}
		if (current == NULL)
		{
			return 0;
		}
		// Agrega direcciones a la tabla de direcciones de los procesos
		add_dir(&(current->direction_table), data_array[1], data_array[2]);
	}
	fclose(fp);
	return 1;
}

void print_memoria() //Impresión de la memoria real, memoria es un arreglo dimensional que contiene el proceso, la pagina y su frecuencia
{
	printf("-------------------------------------------\n");
	printf("Memoria Real\n");
	printf("Marco\tProceso\tPagina\tFrecuencia\n");
	for (int i = 0; i < 5; i++)
	{
		printf("%d\t", i);
		for (int j = 0; j < 3; j++)
		{
			if (memoria[i][j] != -1)
			{
				printf("%d\t", memoria[i][j]);
			}
			else
			{
				printf("-\t");
			}
		}
		printf("\n");
	}
}

int check_page(int num_process, int page) //Función que se encarga de revisar la inexistencia de pagina,
{
	Row_process_table *current = proc_table.first_row;
	while (current != NULL)
	{
		if (current->num_process == num_process)
		{
			if ((page + 1) > current->num_pages)
			{
				return 0;
			}
			return 1;
		}
		current = current->next_row;
	}
	return 0;
}

int find_in_mem(int num_process, int page) //Función que busca en memoria el marco de pagina donde se encuentras el proceso y su pagina
{
	for (int i = 0; i < 5; i++)
	{
		if (memoria[i][0] == num_process && memoria[i][1] == page)
		{
			return i;
		}
	}
	return -1;
}

void mod_frec(int marco_pag) //Función que incrementa la frecuencia de un proceso, recibe como parametro el marco de pagina
{
	memoria[marco_pag][2] = memoria[marco_pag][2] + 1;
}

int busca_espacio_mem() //Función que busca espacio disponible en memoria, al salir un proceso deja el valor de -1 para que otro pueda tomar su lugar, retorna el marco de pagina donde hay espacio en memoria
{
	for (int i = 0; i < 5; i++)
	{
		if (memoria[i][0] == -1)
		{
			return i;
		}
	}
	return -1;
}

int cambia_pag(int num_proceso, int num_pag) //Funcion que hace el cambio de pagina y hace un reset a las frecuencias
{
	// Encontrar al registro con menor frecuencia
	int menor = 0;
	for (int i = 1; i < 5; i++)
	{
		if (memoria[i][2] < memoria[menor][2])
		{
			menor = i;
		}
	}
	printf("Sustitucion en el marco de pagina: %d\n", menor);
	printf("Pagina %d de proceso %d sustituida por pagina %d de proceso %d\n",
				 memoria[menor][1], memoria[menor][0], num_pag, num_proceso);
	memoria[menor][0] = num_proceso;
	memoria[menor][1] = num_pag;
	memoria[menor][2] = 0;
	// Reset a las frecuencias
	reset_frecuencias();
	return menor;
}

void reset_frecuencias() //Reinicia las frecuencias de los procesos en memoria
{
	for (int i = 0; i < 5; i++)
	{
		memoria[i][2] = 0;
	}
}

void borra_pags(int num_proceso) //Función que borra la pagina y el proceso que estaba en memoria real
{
	for (int i = 0; i < 5; i++)
	{
		if (memoria[i][0] == num_proceso)
		{
			memoria[i][0] = -1;
			memoria[i][1] = 0;
		}
	}
	reset_frecuencias();
}

void print_ejecucion(int num_proceso) //Imprime el proceso que entra en ejecucion de planificación
{
	if (num_proceso == -1)
		return;
	printf("-------------------------------------------\n");
	printf("Proceso %d entra a ejecucion\n", num_proceso);
}

void termina_proceso(int num_proceso) //Función que se encarga de terminar el proceso y sacarlo de la cola
{
	printf("-------------------------------------------\n");
	printf("El proceso %d ha terminado\n", num_proceso);
	borra_pags(num_proceso);
}

int revisa_desbordamiento(int num_proceso, int num_pag, int offset) //Función que se encarga del error de desbordamiento de pagina
{
	if (offset >= 20)
	{
		printf("-------------------------------------------\n");
		printf("DESBORDAMIENTO DE PAGINA PROCESO: %d\n", num_proceso);
		printf("Causado por la direccion virtual: (%d, %d)\n", num_pag, offset);
		termina_proceso(num_proceso);
		return 1;
	}
	return 0;
}

void proceso_inexist(int num_proceso, int num_pag, int offset) //Función que se encarga del error de la inexistencia de página
{
	printf("-------------------------------------------\n");
	printf("INEXISTENCIA DE PAGINA PROCESO: %d\n", num_proceso);
	printf("Causado por la direccion virtual: (%d, %d)\n", num_pag, offset);
	termina_proceso(num_proceso);
}

int fallo_pagina(int num_proceso, int num_pag, int offset) //Función que se encarga del fallo de pagina
{
	int marco_pag;
	printf("-------------------------------------------\n");
	printf("FALLO DE PAGINA PROCESO: %d\n", num_proceso);
	printf("Causado por la direccion virtual: (%d, %d)\n", num_pag, offset);
	int espacio = busca_espacio_mem();
	if (espacio == -1) //si no hay espacio en memoria se hace un cambio de pagina
	{
		marco_pag = cambia_pag(num_proceso, num_pag);
	}
	else //Si hay espacio en memoria se inserta la pagina y el proceso en el marco de pagina disponible
	{
		printf("Existe al menos un marco de pagina libre\n");
		printf("No fue necesario realizar una sustitucion de paginas\n");
		printf("Pagina %d de proceso %d entra a marco %d\n", num_pag, num_proceso, espacio);
		memoria[espacio][0] = num_proceso;
		memoria[espacio][1] = num_pag;
		reset_frecuencias();
		marco_pag = espacio;
	}
	return marco_pag;
}

void planificador() //Función que se encarga de la planificación de procesos
{
	int num_process = dequeue(&cola_procesos);
	Dir_table *tabla_direcciones = NULL;
	int num_pag;
	int offset;
	int marco_pag;
	int count = 0;
	int dir_real;
	print_ejecucion(num_process);
	while (num_process != -1)
	{
		// Obtener su tabla de direcciones
		tabla_direcciones = find_dir_table(proc_table, num_process);
		// Si no hay direcciones para traducir
		if (tabla_direcciones->first_row == NULL)
		{
			num_process = dequeue(&cola_procesos);
			continue;
		}
		// Obteniendo la direccion virtual de la tabla de direcciones
		num_pag = tabla_direcciones->first_row->page;
		offset = tabla_direcciones->first_row->offset;
		// Revisa desbordamiento de pagina
		if (revisa_desbordamiento(num_process, num_pag, offset))
		{
			num_process = dequeue(&cola_procesos);
			print_ejecucion(num_process);
			continue;
		}
		// Revisa inexistencia de pagina
		if (!check_page(num_process, num_pag))
		{
			proceso_inexist(num_process, num_pag, offset);
			num_process = dequeue(&cola_procesos);
			print_ejecucion(num_process);
			continue;
		}
		// Revisa fallo de pagina
		marco_pag = find_in_mem(num_process, num_pag);
		if (marco_pag == -1)
		{
			marco_pag = fallo_pagina(num_process, num_pag, offset);
		}
		// Traduce direccion virtual a direccion real
		dir_real = marco_pag * 20 + offset;
		//Llama a funcion que aumente la frecuencia
		mod_frec(marco_pag);
		print_memoria();
		printf("Direccion virtual: (%d, %d) | Direccion real: %d\n", num_pag, offset, dir_real);
		delete_head(tabla_direcciones);
		count++;
		if (count >= quantum || tabla_direcciones->first_row == NULL)
		{
			// Si todavía no termina el proceso, vuelve a la cola
			if (tabla_direcciones->first_row != NULL)
			{
				enqueue(&cola_procesos, num_process);
			}
			else
			{
				termina_proceso(num_process);
			}
			num_process = dequeue(&cola_procesos);
			print_ejecucion(num_process);
			count = 0;
		}
	}
}
