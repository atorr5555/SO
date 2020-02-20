/**
 *  José Antonio Torres Verástegui
 *  12/Febrero/2020
 *  Sistemas Operativos - Grupo 4
 *  Descripción: El padre genera un arreglo de 10 elementos con valores entre 0 y 255, crea dos hijos y espera a que terminen y le
 *  devuelvan resultados: el hijo 1 calcula la moda de los elementos del arreglo, el hijo 2 obtiene el mayor valor del arreglo. Los
 *  valores obtenidos son regresados al padre y el los muestra en pantalla. 
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int moda (int array[]);
int mayor (int array[]);
void swap (int *a, int *b);
void printArray(int array[]);

void main () {
    int pid1, pid2, modaNum, mayorNum, pidW1, pidW2, estado1, estado2;
    int array[10] = {118, 9, 220, 123, 118, 9, 118, 12, 53, 100};
    printArray(array);
    printf("PID del padre: %d \n", getpid());
    pid1 = fork(); // Se crea el hijo 1 y se obtiene su PID
    if (pid1) {
        pid2 = fork(); // Se crea el hijo 2 y se obtiene su PID
        if (pid2) {
            pidW1 = wait(&estado1); // El padre obtiene el PID del hijo afectado por el primer wait y espera a que termine para continuar
            pidW2 = wait(&estado2); // El padre obtiene el PID del hijo afectado por el segundo wait y espera a que termina para continuar
            if (pidW1 == pid1) { // Si el hijo afectado por el primer wait es el hijo uno, entonces el estado1 dará el valor de la moda y el estado2 el del mayor
                modaNum = (estado1 >> 8);
                mayorNum = (estado2 >> 8);
            }
            else { // Si el hijo afectado por el primer wait es el hijo dos, entonces el estado2 dará el valor de la moda  y el estado1 el del mayor
                mayorNum = (estado1 >> 8);
                modaNum = (estado2 >> 8);
            }
        }
        else {
            printf("Hijo 2 terminó con PID: %d \n", getpid());
            exit(mayor(array)); // Línea ejecutada por el hijo 2
        }
    }
    else {
        printf("Hijo 1 terminó con PID: %d \n", getpid());
        exit(moda(array)); // Línea ejecutada por el hijo 1
    }
    printf("La moda es: %d \n", modaNum);
    printf("El número mayor: %d \n", mayorNum);

}

// Se obtiene la moda del arreglo
int moda (int array[]) {
    int contador[256] = {0};
    for (int i = 0; i < 10; i++) {
        contador[array[i]] += 1;
    }
    int modaTemp = 0;
    for (int i = 0; i < 256; i++) {
        if (contador[i] > contador[modaTemp]) {
            modaTemp = i;
        }
    }
    return modaTemp;
}

// Se obtiene el elemento mayor del arreglo
int mayor (int array[]) {
    for (int i = 0;i < 9;i++) {
        if (array[i] > array[i+1])
            swap(&array[i], &array[i+1]);
    }
    return array[9];
}

// Función auxiliar que intercambia dos elementos de un arreglo
void swap(int *a, int*b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Función que muestra el arreglo en pantalla
void printArray(int array[]) {
    printf("Arreglo:");
    for (int i = 0; i < 10; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
}