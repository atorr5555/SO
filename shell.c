#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void separar (char *input, char **argv);
void ejecutar (char **argv);
int isPipe(char **argv);
void ejecutaSinPipe(char **argv);
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
        separar(input, argv);
        if (strcmp(*argv, "salir")) {
            ejecutar(argv);
        }
        else {
            exit(0);
        }
    }
}

// Modificar la función
void separar (char *input, char **argv) {
    while (*input != '\0') {
        while (*input == ' '){ // Revisa si hay espacios o tabulaciones
            *input = '\0'; // Camia espacios por caracteres nulos
            input++; // Avanza al siguiente elemento
        }
        *argv = input; // Guarda la ubicación de la palabra
        argv++; // Avanza al siguiente elemento
        while (*input != '\0' && *input != ' ') { //Si no hay espacios, tabuladores, o caracteres vacíos (insertados por los espacios)
            input++; //Avanza al siguiente caracter de la palabra
        }
    }
    *argv = (char *) 0; //El último elemento es un carácter nulo
}

//Revisar el caso para exit
void ejecutar (char **argv) {
    if (isPipe(argv)) {
        char *beforePipe[64];
        char *afterPipe[64];
        ejecutaConPipe(argv, beforePipe, afterPipe);
    }
    else {
        ejecutaSinPipe(argv);
    }
}

void ejecutaConPipe(char **argv, char **beforePipe, char **afterPipe) {
    /*
        Esto no sirve Si quieres arreglalo o haz otra cosa, cambia todo lo que quieras, porque creo que en unas partes mi lógica está dlv
    separaPipe(argv, beforePipe, afterPipe);
    int pid, pid2, pipefd[2];
    pipe(pipefd);
    pid = fork();
    if (pid) {
        pid2 = fork();
        if (!pid2) { //Revisar por qué no funciona esta parte
            close(1); // 1 es la salida estándar
            close(pipefd[0]);
            dup(pipefd[1]); // La salida estándar va a ser pipefd[1]
            execvp(*beforePipe, beforePipe);
        }
        else {
            close(0);
            close(pipefd[1]);
            dup(pipefd[0]);
            execvp(*afterPipe, afterPipe);
        }
    }
    else {
        exit(1);
    }*/
}

void ejecutaSinPipe(char **argv) {
    int pid;
    int estado;
    pid = fork();
    if (pid) {
        wait(&estado);
    }
    else {
        execvp(*argv, argv);
        printf("Error, comando: %s no encontrado \n", *argv);
        exit(0);
    }
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
                *beforePipe++;
            }
        }
        *argv++;
    }
}

//Revisa si hay pipe en la instrucción
//Cambiar esto, a lo mejor ya no se necesita
int isPipe(char **argv) {
    while (*argv != (char *) 0) {
        if (**argv == '|') {
            return 1;
        }
        *argv++;
    }
}