#include <stdio.h>
#include <unistd.h>

int fibonacci (int n);
int factorial (int n);
int validar (int n);

void main() {
    int pid1, pid2, n, fact, fib;
    printf("Ingrese un numero entre 1 y 10: ");
    scanf("%d", &n);    
    while(validar(n) == 0) {
        printf("Incorrecto. Ingrese un numero entre 1 y 10: ");
        scanf("%d", &n);
    }
    pid1 = fork();
    if (pid1 != 0) {
        pid2 = fork();
        if (pid2 == 0) {
            fact = factorial(n);
            printf("El factorial de %d es %d. Calculado por el hijo 2 con PID = %d \n", n, fact, getpid());
        }
    }
    else {
        fib = fibonacci(n);
        printf("El elemento %d de Fibonacci es %d. Calculado por el hijo 1 con PID = %d \n", n, fib, getpid());
    }
}

int fibonacci (int n) {
    if ((n == 0) || (n == 1)) {
        return n;
    }
    return fibonacci(n-1) + fibonacci(n-2);
}

int factorial (int n) {
    if ((n == 0) || (n == 1)) {
        return 1;
    }
    return n*factorial(n-1);
}

int validar (int n) {
    if ((n <= 10) && (n >= 0)) {
        return 1;
    }
    return 0;
}