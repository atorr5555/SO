/*Flores Fuentes Kevin    Torres Verástegui José Antonio   Fecha de elaboración: 20/Febrero/2020*/
/*
Programa shell que se encarga de ejecutar el comando dado, puede ejecutar dos comandos en una sola linea gracias al pipe, el programa funciona a través de apuntadores para referirse a la linea del comando dado y poder separarla de ser el caso,la función execvp() que ejecuta el comando dado, en caso de una fallo devuelve un valor negativo y de esta manera identificamos si hay un comando erroneo o inexistente, y por último e importante la función fork para crear procesos y poder ejecutarlos de manera efectiva a través del proceso padre y sus posibles procesos hijos.
El comando en si pide el comando a ejecutar, después de ello entra a una función llamada ejecutar donde se crean los procesos padre e hijo el padre espera a que acabe el hijo, el hijo por su parte identifica si el comando contiene un pipe o no, de ser el caso entra a la función separaPipe donde esta misma se obtendrá los dos comandos, el comando izquierdo lo ejecutará el hijo mientras que el comando derecho lo ejecutara un nieto creado por el proceso hijo a continuación se detalla mejor el código.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void separar (char *input, char **argv);/*Función que se encarga de obtener el comando sin imperfecciones como espacios*/
void ejecutar (char input[1024]);/*Función que se encarga de ejecutar todos los comandos después de ser dados*/
int isPipe(char **argv);/*Función que identifica si el comando dado cuenta con pipe*/
void separaPipe(char **argv,char **beforePipe, char **afterPipe);/*Función que  separa y obtiene los dos comandos si existe el pipe*/

void main () {
    char input[1024];/*Pedirá un comando mientras la salida no sea 1, esto solo sucederá cuando se coloque salir*/
    char *argv[64];

    while (1) {
        printf("Comando ->: ");
        fgets(input, sizeof(input), stdin);
        char *p = strchr(input, '\n');
        if (p)  *p = '\0'; //Cambia el salto de línea del final de la cadena por un caracter nulo
        ejecutar(input);/*Manda el comando a ejecutar a la función ejecutar*/ 
    }
}

void ejecutar (char input[1024]) {
    char *argv[64];
    pid_t pid1, pid2;/*Crea los pids*/
    pid1 = fork(); /*Crea el proceso hijo*/
    if (pid1) {
        //padre
        int estado = 0;
        wait(&estado);/*Esperará a que el proceso hijo acabe su proceso*/
        if ((estado >> 8) == 24) {
            exit(1);/*Si el proceso regresa un estado de 24 significa que se ha escrito salir,acaba el proceso y manda 1 para acabar el código*/
        }
    }
    else {
        //hijo
        separar(input, argv);/*Separa el comando de salto de lineas y tabulaciones*/
        if (!strcmp(*argv, "salir")) {/*SI el comando de entrada es salir se acaba el proceso y mando 24 para acabar el codigo*/
            exit(24); // For Kobe
        }
        if (isPipe(argv)) {/*Si el comando contiene | la función regresa 1 y entra en el if*/
            char *beforePipe[64], *afterPipe[64];/*Crea dos arreglos para guardar los dos comandos (antes y después del pipe)*/
            int pipefd[2];
            separaPipe(argv, beforePipe, afterPipe);/*La función separa los comandos y los guarda en beforepipe y afterpipe respectivamente*/
            pipe(pipefd);/*Crea el pipe*/
            pid2 = fork();/*Creación del proceso nieto*/
            if (pid2) {
                //hijo
                int estado = 0;
                wait(&estado);
                close(0);// Cierra la entrada estándar
                close(pipefd[1]);
                dup(pipefd[0]);// Entrada estándar es el pipe
                execvp(*afterPipe, afterPipe);/*Ejecuta el comando después del |*/
                if ((estado >> 8) == 8) {
                    printf("Error, comando '%s' no encontrado \n", *beforePipe);
                }
                printf("Error, comando '%s' no encontrado \n", *afterPipe);
                exit(0);
            }
            else {
                //nieto
                close(1); //Se cierra la salida estándar
                close(pipefd[0]);
                dup(pipefd[1]); // La salida estándar va a ser pipefd[1]
                execvp(*beforePipe, beforePipe);/*Ejecuta el comando antes del |*/
                exit(8);
            }
        }
        else {
            execvp(*argv, argv);
            printf("Error, comando '%s' no encontrado \n", *argv);
            exit(0);
        }
    }
}


void separar (char *input, char **argv) {
    while (*input != '\0') {
        while (*input == ' '){ // Revisa si hay espacios
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
    int flag = 0; // Bandera para saber si ya se pasó el punto en elq ue está el pipe
    while (*argv != (char *) 0) {
        if (**argv == '|') { // Si se encuentra el pipe
            flag = 1;
        }
        else {
            if (flag) {
                *afterPipe = *argv; //Comando que están después del pipe
                *afterPipe++;
            }
            else {
                *beforePipe = *argv;//Comando que está antes del pipe
                char *p = strchr(*beforePipe, '|'); // Revisa si el pipe está en el comando para casos en los que no hay espacios entre el pipe
                if (p){
                    *p = '\0'; // Cambia el pipe por nulo
                    flag = 1;
                    *argv++;
                }
                *beforePipe++;
            }
        }
        *argv++;
    }
    *beforePipe = (char *) 0; //Agraga caracteres nulos al final de los vectores de comandos
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

/*Conclusiones:
Kevin FLores Fuentes:
La importencia de la comprensión de las llamadas al sistema es muy importante, 
y más para la materia de SO, creo que este programa de shell hizo que comprendiera
mejor el uso adecuado de la creación de procesos con fork(), asi como el uso debido
de las entradas y salidas con pipe, y por supuesto como execvp() lográ ejecutar un
comando en linux, me pareció un buen programa para comprender y mejorar la
iniciación de las llamadas al sistema.

José Antonio Torres Verástegui:
El desarrollo de este programa fue de gran utilidad para comprender y aplicar el uso
de las llamadas al sistema vistas en clase, y sobre todo por desarrollar un elemento
de software tan importante como lo es un shell (intérprete de comandos). Desde mi punto
de vista, la mayor dificultad de este software radicó en separar las palabras introducidas
(comandos) e identificar el pipe, sobre todo porque el pipe  puede estar o no entre espacios.

*/