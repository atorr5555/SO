/**
 *  Ejercicio en clase
 *  Se muestra el directorio, se borra un archivo y se vuelve a mostrar
 *  
**/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void main() {
    int pid, pid2;
    pid = fork();
    if (pid) {
        wait(NULL);
        execlp("ls", "ls", "-l", (char *) 0);
    }
    else {
        // execlp es exclusivo para ejecutar comandos
        pid2 = fork();
        if (pid2) {
            wait(NULL);
            execlp("rm", "rm", "hola.c", (char *) 0);
        }
        execlp("ls", "ls", "-l", (char *) 0);
    }
}