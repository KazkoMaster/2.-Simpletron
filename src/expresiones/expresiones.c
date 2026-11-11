#include <stdio.h>
#include "pila.h"
#include "expresiones.h"
#include <string.h>
#include <ctype.h>

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

static int esOperador(char c){
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

static int precedencia(char operador){
    if(operador == '^'){
        return 3;
    }
    if(operador == '*' || operador == '/'){
        return 2;
    }
    return 1;
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

int infijaAPostfija(const char infija[], char postfija[]){

    Pila pila;
    int i = 0;
    int j = 0;
    int simbolo;
    char c;
    int esperaOperando = 1;

    postfija[0] = '\0';

    if(strlen(infija) >= TAM_EXPRESION){
        printf("*** Error: la expresion es demasiado larga ***\n");
        return 0;
    }

    if(!verificarAgrupacion(infija)){
        return 0;
    }

    inicializarPila(&pila);

    while(infija[i] != '\0'){

        c = infija[i];

        if(c == ' '){
            i++;
            continue;
        }

   if(isdigit((unsigned char) c) || isalpha((unsigned char) c)){

            if(!esperaOperando){
                printf("*** Error: falta un operador antes de la posicion %d ***\n", i);
                return 0;
            }

            if(isdigit((unsigned char) c)){
                while(isdigit((unsigned char) infija[i])){
                    postfija[j++] = infija[i++];
                }
            } else{
                postfija[j++] = infija[i++];
                if(isalnum((unsigned char) infija[i])){
                    printf("*** Error: las variables son de una sola letra (posicion %d) ***\n", i);
                    return 0;
                }
            }

            postfija[j++] = ' ';
            esperaOperando = 0;
            continue;
        }

        if(esApertura(c)){
            if(!esperaOperando){
                printf("*** Error: falta un operador antes de la posicion %d ***\n", i);
                return 0;
            }

            if(!push(&pila, c)){
                return 0;
            }
        } else if(esCierre(c)){

            if(esperaOperando){
                printf("*** Error: falta un operando antes de la posicion %d ***\n", i);
                return 0;
            }

            pop(&pila, &simbolo);
            while(!esApertura((char) simbolo)){
                postfija[j++] = (char) simbolo;
                postfija[j++] = ' ';
                pop(&pila, &simbolo);
            }
        } else if(esOperador(c)){

            if(esperaOperando){
                printf("*** Error: falta un operando antes de la posicion %d ***\n", i);
                return 0;
            }

            while(!isEmpty(&pila)){
                stackTop(&pila, &simbolo);
                if(!esOperador((char) simbolo)){
                    break;
                }

                if(precedencia((char) simbolo) > precedencia(c) || (precedencia((char) simbolo) == precedencia(c) && c != '^')){
                    pop(&pila, &simbolo);
                    postfija[j++] = (char) simbolo;
                    postfija[j++] = ' ';
                } else{
                    break;
                }
            }

            if(!push(&pila, c)){
                return 0;
            }
            esperaOperando = 1;

        } else{
            printf("*** Error: caracter invalido '%c' en la posicion %d ***\n", c, i);
            return 0;
        }
        i++;
    }

    if(esperaOperando){
        printf("*** Error: la expresion termina sin operando ***\n");
        return 0;
    }

    while(!isEmpty(&pila)){
        pop(&pila, &simbolo);
        postfija[j++] = (char) simbolo;
        postfija[j++] = ' ';
    }

    if(j > 0){
        j--;
    }
    postfija[j] = '\0';

    return 1;
}