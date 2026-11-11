#include <stdio.h>
#include "pila.h"
#include "expresiones.h"

static int esApertura(char c){
    return c == '(' || c == '[' || c == '{';
}

static int esCierre(char c){
    return c == ')' || c == ']' || c == '}';
}

static char aperturaDe(char cierre){
    if(cierre == ')'){
        return '(';
    }
    if(cierre == ']'){
        return '[';
    }
    return '{';
}


/* Regresa la profundidad maxima de anidamiento de la expresion,
 * o -1 si en algun punto el conteo es negativo (un cierre sin su
 * apertura) o si al final no es cero (aperturas sin cerrar). */
int profundidad(const char expresion[]){

    int i;
    int actual = 0;
    int maximo = 0;

    for(i = 0; expresion[i] != '\0'; i++){
        if(esApertura(expresion[i])){
            actual++;
            if(actual > maximo){
                maximo = actual;
            }
        } else if(esCierre(expresion[i])){
            actual--;
            if(actual < 0){
                return -1;
            }
        }
    }

    if(actual != 0){
        return -1;
    }

    return maximo;
}

/* Verifica con una pila que cada simbolo de cierre corresponda
 * con el ultimo simbolo de apertura: ( con ), [ con ] y { con }.
 * Regresa 1 si la agrupacion es correcta y 0 si no. */

int verificarAgrupacion(const char expresion[]){

    Pila pila;
    int i;
    int simbolo;
    char c;

    inicializarPila(&pila);

    for(i = 0; expresion[i] != '\0'; i++){

        c = expresion[i];

        if(esApertura(c)){
            if(!push(&pila, c)){
                return 0;
            }
        } else if(esCierre(c)){

            if(isEmpty(&pila)){
                printf("*** Error: '%c' en la posicion %d no tiene simbolo de apertura ***\n", c, i);
                return 0;
            }

            pop(&pila, &simbolo);

            if(simbolo != aperturaDe(c)){
                printf("*** Error: '%c' en la posicion %d no corresponde con '%c' ***\n", c, i, simbolo);
                return 0;
            }
        }
    }

    if(!isEmpty(&pila)){
        stackTop(&pila, &simbolo);
        printf("*** Error: falta cerrar '%c' ***\n", simbolo);
        return 0;
    }

    return 1;
}

