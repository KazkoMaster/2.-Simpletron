#include <stdio.h>
#include "pila.h"

void probarPila(void);

int main(void){
    probarPila();
    return 0;
}

void probarPila(void){
    Pila pila;
    int valor;
    int i;

    printf("==== Pruebas de la pila ====\n");

    inicializarPila(&pila);
    printf("Pila nuvea vacia: %s\n", isEmpty(&pila) ? "si" : "no" );

    push(&pila, 10);
    push(&pila, 20);
    push(&pila, 30);

    stackTop(&pila, &valor);
    printf("Tope despues de push 10, 20, 30: %d\n", valor);

    pop(&pila, &valor);
    printf("pop: %d\n", valor);
    pop(&pila, &valor);
    printf("pop: %d\n", valor);
    pop(&pila, &valor);
    printf("pop: %d\n", valor);

    printf("Vacia despues de 3 pop: %s\n", isEmpty(&pila) ? "si" : "no");

    printf("Prueba de underflow: ");
    pop(&pila, &valor);

    printf("Prueba de overflow (%d push): ", TAM_PILA + 1);
    for(i = 0; i <= TAM_PILA; i++){
        if(!push(&pila, i)){
            break;
        }
    }

    printf("\n");
}