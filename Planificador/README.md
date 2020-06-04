# Programa: Administración de memoria y Planificación de procesos

Programa para la asignatura de Sistemas Operativos

Objetivo del programa: Elaborar un programa en Ansi C que realice la planificación de procesos por la 
técnica de round robin y la administración de memoria por el método de 
paginación por demanda y la de reemplazo de la menos frecuentemente utilizada.

Como entrada, será un archivo con el siguiente formato:
  * Un registro maestro con los datos de cada proceso, de la siguiente forma:
    * en la primera columna tendrá un 0 (para indicar que es un registro maestro)
    * el pid del proceso (un número entero)
    * el tamaño del proceso (un entero)
 * Registros con las direcciones virtuales correspondientes al proceso del registro maestro que le antecede, con los
    * siguientes datos:
    * en la primera columna tendrá un 1 (para indicar que contiene una dirección virtual)
    * la página
    * el desplazamiento

El programa realizará una simulación de un planificador de procesos por round robin
donde el quantum será el tiempo que se tarde en traducir 4 direcciones. La administración
de memoria se realizará por paginación por demanda y reemplazo de la menos frecuentemente utilizada.

## Compilación

Para compilar el programa se debe utilizar la opción -lm debido a que se está utilizando la
biblioteca math.h. Ejemplo:

```
gcc planificador.c -o proyecto -lm
```

## Ejecución

Se debe indicar la ruta relativa o absoluta del archivo de texto de entrada.

```
./proyecto /home/username/Escritorio/Planificador/prueba.txt
```

ó

```
./proyecto prueba.txt
```