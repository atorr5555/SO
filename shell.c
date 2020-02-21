#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void separar (char *input, char **argv);
void ejecutar (char input[1024]);
int isPipe(char **argv);
void separaPipe(char **argv,char **beforePipe, char **afterPipe);
void ejecutaConPipe(char **argv, char **beforePipe, char **afterPipe);

void main () {
    char input[1024];
    char *argv[64];

    while (1) {
        printf("Comando ->: ");
        fgets(input, sizeof(input), stdin);
        char *p = strchr(input, '\n');
        if (p)  *p = '\0'; //Cambia el salto de línea del final de la cadena por un caracter nulo
        ejecutar(input);
    }
}

void ejecutar (char input[1024]) {
    char *argv[64];
    pid_t pid1, pid2;
    pid1 = fork();
    if (pid1) {
        //padre
        int estado = 0;
        wait(&estado);
        if ((estado >> 8) == 24) {
            exit(1);
        }
    }
    else {
        //hijo
        separar(input, argv);
        if (!strcmp(*argv, "salir")) {
            exit(24); // For Kobe
        }
        if (isPipe(argv)) {
            char *beforePipe[64], *afterPipe[64];
            int pipefd[2];
            separaPipe(argv, beforePipe, afterPipe);
            pipe(pipefd);
            pid2 = fork();
            if (pid2) {
                //hijo
                close(0);
                close(pipefd[1]);
                dup(pipefd[0]);
                execvp(*afterPipe, afterPipe);
                printf("Error, comando '%s' no encontrado \n", *afterPipe);
            }
            else {
                //nieto
                close(1); // 1 es la salida estándar
                close(pipefd[0]);
                dup(pipefd[1]); // La salida estándar va a ser pipefd[1]
                execvp(*beforePipe, beforePipe);
                printf("Error, comando '%s' no encontrado \n", *beforePipe);
            }
        }
        else {
            execvp(*argv, argv);
            printf("Error, comando '%s' no encontrado \n", *argv);
        }
    }
}

// Modificar la función
void separar (char *input, char **argv) {
    while (*input != '\0') {
        while (*input == ' '){ // Revisa si hay espacios o tabulaciones
            *input = '\0'; // Cambia espacios por caracteres nulos
            input++; // Avanza al siguiente elemento
        }
        *argv = input; // Guarda la ubicación de la palabra
        argv++; // Avanza al siguiente elemento
        while (*input != '\0' && *input != ' ') { //Si no hay espacios, tabuladores, o caracteres vacíos (insertados por los espacios)
            if (*input == '|') { // Caso en el que hay un pipe pero no hay espacios antes y después del pipe
                *argv = input; // Se guarda la posición del pipe
                argv++;
                input++;
                break; //Se sigue a revisar la siguiente palabra
            }
            input++; //Avanza al siguiente caracter de la palabra
        }
    }
    *argv = (char *) 0; //El último elemento es un carácter nulo
}

void separaPipe(char **argv,char **beforePipe, char **afterPipe) {
    int flag = 0;
    while (*argv != (char *) 0) {
        if (**argv == '|') {
            flag = 1;
        }
        else {
            if (flag) {
                *afterPipe = *argv;
                *afterPipe++;
            }
            else {
                *beforePipe = *argv;
                char *p = strchr(*beforePipe, '|');
                if (p){
                    *p = '\0';
                    flag = 1;
                    *argv++;
                }
                *beforePipe++;
            }
        }
        *argv++;
    }
    *beforePipe = (char *) 0;
    *afterPipe = (char *) 0;
}

//Revisa si hay pipe en la instrucción
int isPipe(char **argv) {
    while (*argv != (char *) 0) {
        if (**argv == '|') {
            return 1;
        }
        *argv++;
    }
}