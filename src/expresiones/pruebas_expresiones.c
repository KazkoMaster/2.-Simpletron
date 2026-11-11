#include <stdio.h>
#include "pila.h"
#include "expresiones.h"
#include <string.h>

void probarPila(void);
void probarAgrupacion(void);
void probarConversion(void);
void probarEvaluacion(void);
void modoInteractivo(void);

int main(int argc, char *argv[]){
    if (argc == 2 && strcmp(argv[1], "-i") == 0){
        modoInteractivo();
        return 0;
    }
    
    probarPila();
    probarAgrupacion();
    probarConversion();
    probarEvaluacion();
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

void probarAgrupacion(void){
    const char *casos[] = {
        "7 - ((X * ((X + Y) / (J - 3)) + Y) / (4 - 2))",
        "{[a + b] * (c - d)}",
        "(3 + 5 * 4)",
        "2 + 3",
        "((A + B)",
        ")a + b(",
        "(a + b]",
        "{[(a + b)]}"
    };

    int totalCasos = sizeof(casos) / sizeof(casos[0]);
    int i;

    printf("==== Pruebas de agrupacion ====\n");

    for(i = 0; i < totalCasos; i++){
        printf("\nExpresion : %s\n", casos[i]);
        printf("Profundidad maxima: %d\n", profundidad(casos[i]));
        printf("Agrupacion: %s\n", verificarAgrupacion(casos[i]) ? "correcta" : "incorrecta");
    }
    printf("\n");
}

void probarConversion(void){

    const char *casos[] = {
        "3 + 5 * 4",
        "(3 + 5) * 4",
        "10 - 4 - 3",
        "2 ^ 3 ^ 2",
        "100 / (20 - 15) * 3",
        "a * (b + c) - d ^ 2",
        "{[2 + 3] * (4 - 1)}",
        "7-((x*((x+y)/(j-3))+y)/(4-2))",
        "3 +",
        "* 3",
        "3 4",
        "3 $ 4",
        "(3 + 4",
        "ab + 1"
    };

    int totalCasos = sizeof(casos) / sizeof(casos[0]);
    char postfija[TAM_POSTFIJA];
    int i;

    printf("==== Pruebas de conversion infija a postfija ====\n");

    for(i = 0; i < totalCasos; i++){
        printf("\nInfija:   %s\n", casos[i]);
        if(infijaAPostfija(casos[i], postfija)){
            printf("Postfija: %s\n", postfija);
        } else{
            printf("Postfija: (no se pudo convertir)\n");
        }
    }
    printf("\n");
}

void probarEvaluacion(void){

    const char *infijas[] = {
        "3 + 5 * 4",
        "(3 + 5) * 4",
        "10 - 4 - 3",
        "2 ^ 3 ^ 2",
        "100 / (20 - 15) * 3",
        "{[2 + 3] * (4 - 1)}",
        "7 - 10",
        "17 / 5",
        "8 / (4 - 4)",
        "10 ^ 6",
        "x + 1"
    };
    const char *postfijas[] = {
        "4 5 6",
        "4 +",
        "123456 1 +"
    };

    int totalInfijas = sizeof(infijas) / sizeof(infijas[0]);
    int totalPostfijas = sizeof(postfijas) / sizeof(postfijas[0]);
    char postfija[TAM_POSTFIJA];
    int resultado;
    int i;

    printf("==== Pruebas de evaluacion (infija -> postfija -> resultados) ====\n");

    for(i = 0; i < totalInfijas; i++){
        printf("\nInfija:    %s\n", infijas[i]);
        if(!infijaAPostfija(infijas[i], postfija)){
            printf("Resultado: (no se pudo convertir)\n");
            continue;
        }
        printf("Postfija: %s\n", postfija);
        if(evaluarPostfija(postfija, &resultado)){
            printf("Resultado: %d\n", resultado);
        } else{
            printf("Resultado: (no se pudo evaluar)\n");
        }
    }

    printf("\n ---- Postfijas mal formadas ----");

    for (i = 0; i < totalPostfijas; i++)
    {
        printf("\nPostfija:  %s\n", postfijas[i]);
        if(evaluarPostfija(postfijas[i], &resultado)){
            printf("Resutlado: %d\n", resultado);
        } else{
            printf("Resultado: (no se pudo evaluar)\n");   
        }
    }
    printf("\n");
}

void modoInteractivo(void){

    char linea[TAM_EXPRESION];
    char postfija[TAM_POSTFIJA];
    int resultado;
    int largo;

    printf("==== Modo interactivo ====\n");
    printf("Escriba una expresion (linea vacia para salir).\n");

    while(1){

        printf("\n> ");
        fflush(stdout);

        if(fgets(linea, TAM_EXPRESION, stdin) == NULL){
            break;
        }

        largo = strlen(linea);
        if(largo > 0 && linea [largo - 1] == '\n'){
            linea[largo - 1] = '\0';
        }

        if(linea[0] == '\0'){
            break;
        }

        printf("Profundidad maxima: %d\n", profundidad(linea));

        if(!infijaAPostfija(linea, postfija)){
            continue;
        }

        printf("Postfija: %s\n", postfija);

        if(evaluarPostfija(postfija, &resultado)){
            printf("Resultado: %d\n", resultado);
        }
    }
    printf("Fin del modo interactivo. \n");
    
}