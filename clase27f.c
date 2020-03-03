#include <stdio.h>
#include <pthread.h>

void *codigo_hilo(void *id);

void main() {
    pthread_t hilo;
    int error;
    int id=3;
    int *salida;

    error = pthread_create(&hilo, NULL, codigo_hilo, &id); //int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(start_routine)(void*), void *arg)
    if (error) printf("No se pudo crear el hilo");
    error = pthread_join(hilo, (void **)&salida);//int pthread_join(pthread_t thread, void **value_ptr)
    //join es equivalente a wait
    if (error) printf("No se pudieron unir los hilos");
    printf("Hilo terminado, recibi: %d \n", *salida);
}

void *codigo_hilo(void *id) {
    int i;
    for (i = 0; i < 50 ; i++)
        printf("\n Soy el hilo %d, i=%d", *(int *)id, i);
    printf("\n");
    pthread_exit(id);//void pthread_exit(void *value_ptr)
}