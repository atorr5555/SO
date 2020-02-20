/*
    pipe
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void main () {
    int pid, pipes[2]; //pipes[0] es de lectura, pipes[1] es de escritura
    char mensaje1[20], mensaje2[20];
    pipe(pipes);
    pid = fork();
    if (pid) {
        printf("Dame mensaje:");
        fgets(mensaje1, sizeof(mensaje1), stdin);
        char *p = strchr(mensaje1, '\n');
        if (p)  *p = '\0';
        close(pipes[0]);
        write(pipes[1], mensaje1, strlen(mensaje1) + 1);
    }
    else {
        close(pipes[1]);
        read(pipes[0], mensaje2, 20); // Se queda aquí en espera mientras no haya nada en el pipe
        printf("Mensaje recibido: %s \n", mensaje2);
    }
}