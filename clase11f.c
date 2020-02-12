/**
 *  Clase 11 Febrero 2020
 * **/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void main () {
    int i = 10,j = 12, pid, estado;
    pid = fork();
    if (pid) {
        printf("Espero a que mi hijo termine \n");
        wait(&estado);
        printf("Valor recibido del hijo: %d \n", estado >> 8);
    }
    else {
        i += j;
        exit(i);
    }
}