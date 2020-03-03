/*
Elaborar un programa que obtenga la suma de dos vectores de 100 elementos enteros utilizando 5 hilos
*/

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int array1[100];
int array2[100];
int arrayR[100];

void *sumaArreglos (void *indice);
void llenaVectores();

void main() {
    pthread_t hilos[5];
    int error, i;
    int indiceIni;
    llenaVectores();
    for (i = 0 ; i < 5 ; i++) {
        indiceIni = i*20;
        error = pthread_create(&hilos[i], NULL, sumaArreglos, &indiceIni);
    }
    for (i = 0 ; i < 5 ; i++) {
        error = pthread_join(hilos[i], NULL);
    }
    for (i = 0; i < 100; i++) {
        printf("%d \n", arrayR[i]);
    }

}

void *sumaArreglos (void *indice) {
    int inicial = *(int *)indice;
    for (int i = inicial; i < (inicial+20) ; i++) {
        arrayR[i] = array1[i] +array2[i];
    }
    pthread_exit(NULL);
}

void llenaVectores() {
    for (int i = 0 ; i < 100 ; i++) {
        array1[i] = i;
        array2[i] = i;
    }
}