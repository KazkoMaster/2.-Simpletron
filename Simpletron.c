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