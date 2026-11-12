/* ==========================================================
 *  tabla_simbolos.c - Tabla de simbolos del compilador Simple
 *  Cada entrada guarda un simbolo, su tipo y su ubicacion en
 *  la memoria de Simpletron:
 *    L  numero de linea   -> simbolo = numero de linea
 *    V  variable          -> simbolo = codigo ASCII de la letra
 *    C  constante         -> simbolo = valor de la constante
 *    T  temporal          -> simbolo = numero de temporal
 * ========================================================== */

#include <stdio.h>
#include "tabla_simbolos.h"

static EntradaTabla tabla[TAM_TABLA];
static int totalEntradas;

void inicializarTabla(void){
    totalEntradas = 0;
}

/* Regresa la ubicacion del simbolo, o -1 si no esta en la tabla. */
int buscarSimbolo(int simbolo, char tipo){

    int i;

    for(i = 0; i < totalEntradas; i++){
        if(tabla[i].simbolo == simbolo && tabla[i].tipo == tipo){
            return tabla[i].ubicacion;
        }
    }

    return -1;
}

/* Regresa 1 si se agrego y 0 si la tabla esta llena */
int agregarSimbolo(int simbolo, char tipo, int ubicacion){

    if(totalEntradas >= TAM_TABLA){
        printf("*** Error : la tabla de simbolos esta llena ***\n");
        return 0;
    }

    tabla[totalEntradas].simbolo = simbolo;
    tabla[totalEntradas].tipo = tipo;
    tabla[totalEntradas].ubicacion = ubicacion;
    totalEntradas++;
    return 1;
}

/* Busca un dato (variable, constante o temporal) por su ubicacion.
 * Regresa 1 y copia la entrada si lo encuentra, 0 si no. */
int buscaPorUbicacion(int ubicacion, EntradaTabla *entrada){

    int i;

    for(i = 0; i < totalEntradas; i++){

        if(tabla[i].tipo != TIPO_LINEA && tabla[i].ubicacion == ubicacion){
            *entrada = tabla[i];
            return 1;
        }
    }

    return 0;
}

/* Mueve los datos que estan arriba de "limite" hacia abajo
 * "desplazamiento" posiciones (ver reubicarDatos en compilador.c). */
void reubicarTabla(int limite, int desplazamiento){

    int i;

    for(i = 0; i < totalEntradas; i++){
        if(tabla[i].tipo != TIPO_LINEA && tabla[i].ubicacion > limite){
            tabla[i].ubicacion -= desplazamiento;
        }
    }
}

void imprimirTabla(void){

    int i;

    printf("\n===== Tabla de simbolos =====\n");
    printf("%-8s %-12s %s\n", "Tipo", "Simbolo", "Ubicacion");

    for(i = 0; i < totalEntradas; i++){

        switch(tabla[i].tipo){
            case TIPO_LINEA:
                printf("%-8c %-12d %03d\n", tabla[i].tipo, tabla[i].simbolo, tabla[i].ubicacion);
                break;
            case TIPO_VARIABLE:
                printf("%-8c %-12c %03d\n", tabla[i].tipo, (char) tabla[i].simbolo, tabla[i].ubicacion);
                break;
            case TIPO_CONSTANTE:
                printf("%-8c %-12d %03d\n", tabla[i].tipo, tabla[i].simbolo, tabla[i].ubicacion);
                break;
            default:
                printf("%-8c t%-11d %03d\n", tabla[i].tipo, tabla[i].simbolo, tabla[i].ubicacion);
                break;
        }
    }
}