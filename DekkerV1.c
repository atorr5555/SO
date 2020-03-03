#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int num_hilo;

void *codigo_hilo1(void *id) {
    int i = 0;
    while(i < 5) {
        i++;
        while(num_hilo == 2);
        printf("\n hilo1 en region critica \n");
        fflush(stdout);
        sleep(2);
        printf("\n hilo1 fuera de la region critica \n");
        fflush(stdout);
        num_hilo = 2;
    }
    pthread_exit(NULL);
}

void *codigo_hilo2(void *id) {
    int i = 0;
    while(i < 5) {
        i++;
        while(num_hilo == 1);
        printf("\n hilo2 en region critica \n");
        fflush(stdout);
        sleep(1);
        printf("\n hilo2 fuera de la region critica \n");
        fflush(stdout);
        num_hilo = 1;
    }
    pthread_exit(NULL);
}

void main() {
    pthread_t hilo1, hilo2;
    int error;
    int *valor;
    num_hilo = 2;

    error = pthread_create(&hilo1, NULL, codigo_hilo1, NULL);
    error = pthread_create(&hilo2, NULL, codigo_hilo2, NULL);
    error = pthread_join(hilo2, NULL);
    error = pthread_join(hilo1, NULL);
}