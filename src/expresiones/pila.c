#include <stdio.h>
#include "pila.h"

void inicializarPila(Pila *pila) {
    pila->tope = -1;
}

int isEmpty(const Pila *pila){
    return pila->tope == -1;
}

int isFull(const Pila *pila){
    return pila->tope == TAM_PILA - 1;
}

int push(Pila *pila, int valor){

    if(isFull(pila)){
        printf("*** Error: desbordamiento de la pila ***\n");
        return 0;
    }

    pila->tope++;
    pila->elementos[pila->tope] = valor;
    return 1;
}

int pop(Pila *pila, int*valor){
    if(isEmpty(pila)){
        printf("*** Error: pila vacia (underflow) ***\n");
        return 0;
    }

    *valor = pila->elementos[pila->tope];
    pila->tope--;
    return 1;
}

int stackTop(const Pila *pila, int *valor){
    
    if(isEmpty(pila)){
        printf("*** Error: pila vacia (underflow) ***\n");
        return 0;
    }

    *valor = pila->elementos[pila->tope];
    return 1;
}