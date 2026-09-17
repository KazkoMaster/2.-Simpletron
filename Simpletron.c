#include <stdio.h>

#define TAM_MEMORIA 100
#define PALABRA_MIN -9999
#define PALABRA_MAX 9999
#define CENTINELA 9999

int memory[TAM_MEMORIA];
int accumulator;
int instructionCounter;
int instructionRegister;
int operationCode;
int operand;

void mostrarBienvenida(void);
void inicializar(void);
void cargarPrograma(void);
void ejecutarPrograma(void);
void vaciadoMemoria(void);

int main(void){
    mostrarBienvenida();
    inicializar();
    cargarPrograma();
    ejecutarPrograma();
    vaciadoMemoria();

    return 0;

}

void mostrarBienvenida(void){

    printf("*** Bienvenido a Simpletron! ***\n");
    printf("*** Introduzca su programa una instruccion ***\n");
    printf("*** (o palabra de datos) a la vez. Yo indicare ***\n");
    printf("*** el numero de posicion y una interrogacion (?) ***\n");
    printf("*** Ustede tecleara entonces la palabra para esa ***\n");
    printf("*** posicion. Escriba 9999 para dejar de ***\n");
    printf("*** introducir su programa ***\n");

}

void inicializar(void){

    int i;

    for(i = 0; i < TAM_MEMORIA; i++){
        memory[i] = 0;
    }

    accumulator = 0;
    instructionCounter = 0;
    instructionRegister = 0;
    operationCode = 0;
    operand = 0;
}

void cargarPrograma(void){

}

void ejecutarPrograma(void){

}

void vaciadoMemoria(void){

}