#ifndef PILA_H
#define PILA_H

#define TAM_PILA 100

typedef struct{
    int elementos[TAM_PILA];
    int tope;
} Pila;

void inicializarPila(Pila *p);
int isEmpty(const Pila *pila);
int isFull(const Pila *pila);
int push(Pila *pila, int valor);
int pop(Pila *pila, int *valor);
int stackTop(const Pila *pila, int *valor);

#endif