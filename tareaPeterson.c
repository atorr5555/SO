/*  Torres Verástegui José Antonio
    Flores Fuentes Kevin
    Sistemas Operativos
    10-Marzo-2020
    
    El programa realiza una implementación del programa de Peterson
    en el que las regiones críticas son en las que se modifica el saldo
    a través de retiros y depósitos. El hilo 1 maneja los depósitos y
    el hilo 2 maneja los retiros.
*/
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int proc_favorecido, proc1deseaEntrar, proc2deseaEntrar;
float saldo;

/*
    Acciones a realizar por el hilo 1 en la región crítica (saldo)

    Pide la cantidad a depositar y actualiza el saldo
*/
void region_critica_uno () {
    float cant_deposito;
    printf("==========================================\n");
    printf("Ingrese la cantidad a depositar: ");
    fflush(stdout);
    scanf("%f", &cant_deposito);
    if (cant_deposito > 0) { // Si la cantidad a depositar es mayor a 0 actualiza el saldo
        saldo += cant_deposito;
        printf("El saldo es: %.3f\n", saldo);
        fflush(stdout);
    }
    else {// Si la cantidad a depositar no es mayor a 0 el hilo termina
        proc1deseaEntrar = 0;
        pthread_exit(NULL);// El hilo 1 termina
    }
}

void region_critica_dos () {
    float cant_retiro;
    printf("==========================================\n");
    printf("Ingrese la cantidad a retirar: ");
    fflush(stdout);
    scanf("%f", &cant_retiro);
    if (cant_retiro > 0) {// Si la cantidad a retirar es mayor a 0 se actualiza el saldo 
        if (saldo >= cant_retiro) {// Si el saldo es mayor o igual a la cantidad a retirar, se realiza el retiro
            saldo -= cant_retiro;
        }
        else {// Si el saldo no es mayor o igual a la cantidad a retirar, se muestra un mensaje diciendo que no se puede realizar el retiro
            printf("No es posible hacer el retiro\n");
        }
        printf("El saldo es: %.3f\n", saldo);
        fflush(stdout);
    }
    else {// Si la cantidad a retirar no es mayor a 0 termina la ejecución del hilo
        proc2deseaEntrar = 0;
        pthread_exit(NULL);// El hilo 2 termina
    }   
}

/*
    Código a ejecutar por el hilo 1

    Se incluyen las primitivas de entrada y de salida para controlar el acceso a las regiones críticas
*/
void *proc_uno () {
    while (1) {
        proc1deseaEntrar = 1;
        proc_favorecido = 2;
        while (proc2deseaEntrar && proc_favorecido == 2);
        region_critica_uno();
        proc1deseaEntrar = 0;
        //otras_tareas_uno();
    }
}

/*
    Código a ejecutar por el hilo 2

    Se incluyen las primitivas de entrada y de salida para controlar el acceso a las regiones críticas
*/
void *proc_dos () {
    while (1) {
        proc2deseaEntrar = 1;
        proc_favorecido = 1;
        while (proc1deseaEntrar && proc_favorecido == 1);
        region_critica_dos();
        proc2deseaEntrar = 0;
        //otras_tareas_dos();
    }
}

void main () {
    pthread_t hilos[2];
    proc1deseaEntrar = 0;
    proc2deseaEntrar = 0;
    saldo = 1000.00;// Saldo inicial es de 1000.00

    pthread_create(&hilos[0], NULL, proc_uno, NULL);// Se crea el hilo 1
    pthread_create(&hilos[1], NULL, proc_dos, NULL);// Se crea el hilo 2

    pthread_join(hilos[0], NULL);// El hilo principal (proceso principal) espera el hilo 1
    pthread_join(hilos[1], NULL);// El hilo principal (proceso principal) espera el hilo 2
}
/*
Conclusiones Kevin Flores Fuentes

Se logro implementar el algoritmo de peterson con el uso de hilos posix, para 
cumplir el uso adecuado de las regiones críticas y no presentar inconsistencias. 
También se logró observar una posible aplicación que tiene el algoritmo de peterson 
implementado con hilos posix para eliminar inconsistencias en la región crítica.

Conclusiones José Antonio Torres Verástegui

Se logró realizar correctamente la implementación del algoritmo de Peterson con
hilos aplicado a una situación en la que la región crítica es la que modifica
el saldo. Este programa fue un buen ejemplo de la aplicación de los hilos
vistos en clase con el algoritmo de Peterson para manejar regiones críticas.

*/