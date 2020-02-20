
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void main() {
    int pid, pipefd[2];
    pipe(pipefd);
    pid = fork();
    if (!pid) { // si !()
        close(1); // 1 es la salida estándar
        close(pipefd[0]);
        dup(pipefd[1]); // La salida estándar va a ser pipefd[1]
        execlp("ls", "ls", "-l", (char *) 0);
    }
    else {
        close(0);
        close(pipefd[1]);
        dup(pipefd[0]);
        execlp("wc", "wc", (char *) 0);
    }
}