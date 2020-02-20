/*
    Elaborar un programa en C con llamadas al sistema donde el proceso
    padre genere un vector de 10 elementos de tipo entero, el hijo ordene el vector
    y se lo envíe al padre ya ordenado
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void printArray(int vector[10]);

void main() {
    int pid, pipefd[2], v_orig[10] = {10,16,25,32,8,9,3,4,22,46}, v_ord[10] = {0};
    pipe(pipefd);
    pid = fork();
    if (pid) {
        close(pipefd[1]);
        read(pipefd[0], v_ord, sizeof(int)*10);
        printf("Vector original: ");
        printArray(v_orig);
        printf("Vector original: ");
        printArray(v_ord);
    }
    else {
        int aux;
        close(pipefd[0]);
        for (int i = 0 ; i < 10 ; i++) {
            for (int j = i ; j < 10 ; j++) {
                if (v_orig[i] > v_orig[j]) {
                    aux = v_orig[j];
                    v_orig[j] = v_orig[i];
                    v_orig[i] = aux;
                }
            }
        }
        write(pipefd[1], v_orig, sizeof(int)*10);

    }
}

void printArray(int vector[10]) {
    for (int i = 0 ; i < 10 ; i++) {
        printf("%d, ", vector[i]);
    }
    printf("\n");
}