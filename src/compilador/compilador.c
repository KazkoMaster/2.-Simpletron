/* ==========================================================
 *  compilador.c - Compilador de Simple a SML en dos pasadas
 *
 *  Primera pasada: lee el programa Simple linea por linea,
 *  llena la tabla de simbolos y genera las instrucciones SML.
 *  Los saltos a lineas que todavia no aparecen se dejan
 *  incompletos y se marcan en el arreglo flags.
 *
 *  Segunda pasada: completa los saltos marcados en flags,
 *  acomoda los datos justo despues del codigo y escribe el
 *  archivo .sml que carga el simulador.
 *
 *  Memoria: el codigo crece desde la 000 hacia arriba y los
 *  datos (variables, constantes y temporales) desde la 999
 *  hacia abajo. Si se encuentran, no hay memoria suficiente.
 *
 *  Autor: Xavier Hernandez Toledo
 *  Programacion Avanzada - Prof. Roberto Salazar
 * ========================================================== */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "compilador.h"
#include "tabla_simbolos.h"
#include "pila.h"
#include "expresiones.h"

static int sml[TAM_MEMORIA];
static int flags[TAM_MEMORIA];
static int lineaFuente[TAM_MEMORIA];
static int contadorInstrucciones;
static int contadorDatos;
static int totalTemporales;
static int lineaActual;
static int errores;

static void inicializarCompilador(void){

    int i;

    for ( i = 0; i < TAM_MEMORIA; i++)
    {
        sml[i] = 0;
        flags[i] = -1;
        lineaFuente[i] = 0;
    }

    contadorInstrucciones = 0;
    contadorDatos = TAM_MEMORIA - 1;
    totalTemporales = 0;
    lineaActual = 0;
    errores = 0;
    inicializarTabla();
}

static void reportarError(const char mensaje[]){
    printf("*** Error en la linea %d: %s ***\n", lineaActual, mensaje);
    errores++;
}

static int hayEspacio(void){
    if(contadorInstrucciones > contadorDatos){
        reportarError("memoria insuficiente: el codigo y los datos se encontraron");
        return 0;
    }

    return 1;
}

/* Agrega una instruccion SML y regresa su posicion, o -1. */
static int emitir(int operacion, int operando){

    if(!hayEspacio()){
        return -1;
    }

    sml[contadorInstrucciones] = operacion * DIVISOR_OPERANDO + operando;
    lineaFuente[contadorInstrucciones] = lineaActual;
    contadorInstrucciones++;

    return contadorInstrucciones - 1;
}

/* Reserva una palabra en el area de datos y regresa su ubicacion */
static int reservarDato(int valorInicial){

    if(!hayEspacio()){
        return -1;
    }

    sml[contadorDatos] = valorInicial;
    contadorDatos--;

    return contadorDatos + 1;
}

static int esVariable(const char token[]){
    return strlen(token) == 1 && islower((unsigned char) token [0]);
}

/* Regresa 1 si el token es un numero entero con signo opcional dentro
 * del rango permitido, y deja su valor en *valor. */
static int esConstante(const char token[], int *valor){

    int i = 0;
    long numero;

    if(token[0] == '-'){
        i = 1;
    }

    if(token[i] == '\0'){
        return 0;
    }
    
    for (; token[i] != '\0'; i++){
        if(!isdigit((unsigned char) token[i])){
            return 0;
        }
    }

    if(strlen(token) > 7){
        return 0;
    }

    numero = strtol(token, NULL, 10);

    if (numero < CONSTANTE_MIN || numero > CONSTANTE_MAX){
        return 0;
    }
    
    *valor = (int) numero;
    return 1;
    
}

static int ubicacionVariable(char variable){

    int ubicacion = buscarSimbolo(variable, TIPO_VARIABLE);

    if(ubicacion == -1){
        ubicacion = reservarDato(0);
        if (ubicacion != -1){
            agregarSimbolo(variable, TIPO_VARIABLE, ubicacion);
        }
    }
    return ubicacion;
}

static int ubicacionConstante(int valor){

    int ubicacion = buscarSimbolo(valor, TIPO_CONSTANTE);

    if (ubicacion == -1){
        ubicacion = reservarDato(valor);
        if (ubicacion != -1){
            agregarSimbolo(valor, TIPO_CONSTANTE, ubicacion);
        }
    }
    return ubicacion;
}

/* Regresa la ubicacion de una variable o constante, o -1 si el
 * token no es ninguna de las dos. */
static int ubicacionOperando(const char token[]){

    int valor;

    if (esVariable(token)){
        return ubicacionVariable(token[0]);
    }

    if(esConstante(token, &valor)){
        return ubicacionConstante(valor);
    }

    return -1;
}

static int esNumeroDeLinea(const char token[], int *numero){

    int i;

    if(token == NULL || token[0] == '\0'){
        return 0;
    }

    for(i = 0; token[i] != '\0'; i++){
        if(!isdigit((unsigned char) token[i])){
            return 0;
        }
    }

    if(strlen(token) > 5){
        return 0;
    }

    *numero = atoi(token);
    return 1;
}

/* Genera un salto. Si la linea destino ya esta en la tabla se
 * completa de inmediato; si no, se deja el operando en 000 y se
 * marca en flags para resolverlo en la segunda pasada. */
static void generarSalto(int operacion, int lineaDestino){

    int ubicacion = buscarSimbolo(lineaDestino, TIPO_LINEA);
    int posicion;

    if(ubicacion != -1){
        emitir(operacion, ubicacion);
    } else{
        posicion = emitir(operacion, 0);
        if(posicion != -1){
            flags[posicion] = lineaDestino;
        }
    }
}

/* ---------- Un comando por funcion ---------- */

static void compilarInput(const char variable[]){

    if(variable == NULL || !esVariable(variable)){
        reportarError("input necesita una variable de una letra");
        return;
    }

    emitir(OP_READ, ubicacionVariable(variable[0]));
}

static void compilarPrint(const char operando[]){

    int ubicacion;

    if(operando == NULL){
        reportarError("print necesita una variable o constante");
        return;
    }

    ubicacion = ubicacionOperando(operando);

    if(ubicacion == -1){
        reportarError("print necesita una variable o constante");
        return;
    }

    emitir(OP_WRITE, ubicacion);
}

static void compilarGoto(const char destino[]){

    int lineaDestino;

    if(!esNumeroDeLinea(destino, &lineaDestino)){
        reportarError("goto necesita un numero de linea");
        return;
    }

    generarSalto(OP_BRANCH, lineaDestino);
}

static void compilarIf(const char izquierdo[], const char operador[], const char derecho[], const char palabraGoto[], const char destino[]){

    (void) izquierdo;
    (void) operador;
    (void) derecho;
    (void) palabraGoto;
    (void) destino;
    reportarError("if todavia no esta implementado");

}

static void compilarLet(const char variable[], const char igual[], const char expresion[]){

    (void) variable;
    (void) igual;
    (void) expresion;
    reportarError("let todavia no esta implementado");
}

/* ---- Primera pasada ---- */

static int hayMayusculas(const char texto[]){

    int i;

    for(i = 0; texto[i] != '\0'; i++){
        if(isupper((unsigned char) texto[i])){
            return 1;
        }
    }
    return 0;
}

/* Compila una linea y regresa 1 si la linea era "end". */
static int compilarLinea(char linea[], int *ultimaLinea){

    char copia[TAM_LINEA];
    char *numero;
    char *comando;
    char *t1;
    char *t2;
    char *t3;
    char *t4;
    char *t5;
    int numeroLinea;

    strcpy(copia, linea);
    numero = strtok(copia, " \t");

    if(numero == NULL){
        return 0;
    }

    if(!esNumeroDeLinea(numero, &numeroLinea)){
        printf("*** Error: la linea \"%s\" no empieza con un numero de linea ***\n", linea);
        errores++;
        return 0;
    }

    lineaActual = numeroLinea;

    if(numeroLinea <= *ultimaLinea){
        reportarError("los numeros de linea deben ir en orden ascendente");
        return 0;
    }
    
    *ultimaLinea = numeroLinea;
    agregarSimbolo(numeroLinea, TIPO_LINEA, contadorInstrucciones);
    comando = strtok(NULL, " \t");

    if(comando == NULL){
        reportarError("falta el comando");
        return 0;
    }

    if(strcmp(comando, "rem") == 0){
        return 0;
    }

    if(hayMayusculas(linea)){
        reportarError("Simple solo acepta minusculas (salvo en rem)");
        return 0;
    }

    if(strcmp(comando, "let") == 0){
        t1 = strtok(NULL, " \t");
        t2 = strtok(NULL, " \t");
        t3 = strtok(NULL, "");
        compilarLet(t1, t2, t3);
        return 0;
    }

    t1 = strtok(NULL, " \t");
    t2 = strtok(NULL, " \t");
    t3 = strtok(NULL, " \t");
    t4 = strtok(NULL, " \t");
    t5 = strtok(NULL, " \t");

    if(strcmp(comando, "input") == 0 && t2 == NULL){
        compilarInput(t1);
    } else if(strcmp(comando, "print") == 0 && t2 == NULL){
        compilarPrint(t1);
    } else if(strcmp(comando, "goto") == 0 && t2 == NULL){
        compilarGoto(t1);
    } else if(strcmp(comando, "if") == 0 && strtok(NULL, " \t") == NULL){
        compilarIf(t1, t2, t3, t4, t5);
    } else if(strcmp(comando, "end") == 0 && t1 == NULL){
        emitir(OP_HALT, 0);
        return 1;
    } else if(strcmp(comando, "input") == 0 || strcmp(comando, "print") == 0 ||
              strcmp(comando, "goto") == 0 || strcmp(comando, "if") == 0 ||
              strcmp(comando, "end") == 0){
        reportarError("sobran elementos al final de la linea");
    } else{
        reportarError("comando desconocido");
    }

    return 0;
}

static void primeraPasada(FILE *fuente){

    char linea[TAM_LINEA];
    int largo;
    int ultimaLinea = -1;
    int hayEnd = 0;

    while(fgets(linea, TAM_LINEA, fuente) != NULL){

        largo = strlen(linea);
        while(largo > 0 && (linea[largo - 1] == '\n' || linea[largo - 1] == '\r')){
            linea[largo - 1] = '\0';
            largo--;
        }

        if(compilarLinea(linea, &ultimaLinea)){
            hayEnd = 1;
        }
    }

    if(!hayEnd){
        printf("*** Error: el programa no tiene el comando end ***");
        errores++;
    }
}

static void segundaPasada(void){

    int i;
    int ubicacion;

    for(i = 0; i < contadorInstrucciones; i++){

        if(flags[i] != -1){

            ubicacion = buscarSimbolo(flags[i], TIPO_LINEA);
            lineaActual = lineaFuente[i];

            if(ubicacion == -1){
                printf("*** Error en la linea %d: la linea destino %d no existe ***\n", lineaActual, flags[i]);
                errores++;
            } else{
                sml[i] += ubicacion;
            }
        }
    }
}

/* Mueve los datos de la parte alta de la memoria para que queden
 * justo despues del codigo, y corrige los operandos que los usan.
 * Asi el archivo .sml es continuo, como lo carga el simulador. */
static void reubicarDatos(void){

    int desplazamiento = (contadorDatos + 1) - contadorInstrucciones;
    int i;

    for(i = 0; i < contadorInstrucciones; i++){
        if(sml[i] % DIVISOR_OPERANDO > contadorDatos){
            sml[i] -= desplazamiento;
        }
    }

    for(i = contadorDatos + 1; i < TAM_MEMORIA; i++){
        sml[i - desplazamiento] = sml[i];
    }

    reubicarTabla(contadorDatos, desplazamiento);
}

static const char *nombreOperacion(int operacion){

    switch(operacion){

        case OP_READ:       return "READ";
        case OP_WRITE:      return "WRITE";
        case OP_LOAD:       return "LOAD";
        case OP_STORE:      return "STORE";
        case OP_ADD:        return "ADD";
        case OP_SUBTRACT:   return "SUBTRACT";
        case OP_DIVIDE:     return "DIVIDE";
        case OP_MULTIPLY:   return "MULTIPLY";
        case OP_POW:        return "POW";
        case OP_BRANCH:     return "BRANCH";
        case OP_BRANCHNEG:  return "BRANCHNEG";
        case OP_BRANCHZERO: return "BRANCHZERO";
        case OP_HALT:       return "HALT";
        default:            return "?";
    }
}

static void imprimirCodigo(const char titulo[]){

    int i;
    int operacion;

    printf("\n ==== %s ====\n", titulo);

    for(i = 0; i < contadorInstrucciones; i++){

        operacion = sml[i] / DIVISOR_OPERANDO;
        printf("%03d  %+06d  %-10s %03d", i, sml[i], nombreOperacion(operacion), sml[i] % DIVISOR_OPERANDO);

        if(flags[i] != -1){
            printf("   <- salto a la linea %d", flags[i]);
        }
        printf("\n");
    }
}

static int escribirArchivo(const char archivoEntrada[], const char archivoSalida[]){

    FILE *salida;
    EntradaTabla dato;
    int totalDatos = (TAM_MEMORIA - 1) - contadorDatos;
    int i;
    int operacion;

    salida = fopen(archivoSalida, "w");

    if(salida == NULL){
        printf("*** Error: no se pudo crear el archivo %s ***\n", archivoSalida);
        return 0;
    }

    fprintf(salida, "; %s - Generado por el compilador Simple -> SML\n", archivoSalida);
    fprintf(salida, "; Fuente: %s\n", archivoEntrada);
    fprintf(salida, "; Codigo: 000-%03d   Datos:%03d-%03d\n\n", contadorInstrucciones - 1, contadorInstrucciones, contadorInstrucciones+ totalDatos - 1);

    for(i = 0; i < contadorInstrucciones; i++){
        operacion = sml[i] / DIVISOR_OPERANDO;
        fprintf(salida, "%+06d   ; %03d %-10s %03d   (linea %d)\n", sml[i], i, nombreOperacion(operacion), sml[i] % DIVISOR_OPERANDO, lineaFuente[i]);
    }

    fprintf(salida, "\n; ----- Area de datos -----\n");

    for(i = contadorInstrucciones; i < contadorInstrucciones + totalDatos; i++){

        fprintf(salida, "%+06d   ; %03d ", sml[i], i);

        if(buscarPorUbicacion(i, &dato)){
            if(dato.tipo == TIPO_VARIABLE){
                fprintf(salida, "variable %c\n", (char) dato.simbolo);
            } else if(dato.tipo == TIPO_CONSTANTE){
                fprintf(salida, "constante %d\n", dato.simbolo);
            } else{
                fprintf(salida, "temporal t%d\n", dato.simbolo);
            }
        } else{
            fprintf(salida, "\n");
        }
    }

    fprintf(salida, "+99999   ; fin del programa\n");
    fclose(salida);

    printf("\n*** Se genero %s (%d palabras) ***\n", archivoSalida, contadorInstrucciones + totalDatos);
    return 1;
}

/* ----- Funcion principal del modulo -----*/
int compilar(const char archivoEntrada[], const char archivoSalida[]){

    FILE *fuente;
    inicializarCompilador();
    fuente = fopen(archivoEntrada, "r");

    if(fuente == NULL){
        printf("*** Error: no se pudo abrir el archivo %s ***\n", archivoEntrada);
        return 0;
    }

    printf("*** Compilando %s ***\n", archivoEntrada);

    primeraPasada(fuente);
    fclose(fuente);

    imprimirCodigo("Primera pasada (SML preliminar)");
    
    segundaPasada();

    if(errores > 0){
        printf("\n*** Compilacion fallida: %d error(es). No se genero el archivo. ***\n", errores);
        return 0;
    }

    reubicarDatos();
    imprimirCodigo("Segunda pasada (SML Final)");
    imprimirTabla();

    return escribirArchivo(archivoEntrada, archivoSalida);
}

