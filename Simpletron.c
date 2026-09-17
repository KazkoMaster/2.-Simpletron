#include <stdio.h>

#define TAM_MEMORIA 100
#define PALABRA_MIN -9999
#define PALABRA_MAX 9999
#define CENTINELA 9999

#define OP_READ 10
#define OP_WRITE 11
#define OP_LOAD 20
#define OP_STORE 21
#define OP_ADD 30
#define OP_SUBSTRACT 31
#define OP_DIVIDE 32
#define OP_MULTIPLY 33
#define  OP_BRANCH 40
#define OP_BRANCHENG 41
#define OP_BRANCHZERO 42
#define OP_HALT 43

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

    int posicion = 0;
    int palabra;
    int c;

    while (posicion < TAM_MEMORIA){
        printf ("%02d ? ", posicion);

        if(scanf("%d", &palabra) != 1){
            printf("*** Entrada invalida. Escriba un numero entero. ***\n");
            while((c = getchar()) != '\n' && c!= EOF ){

            }
            continue;
        }
        if(palabra == CENTINELA)
            break;

        if(palabra < PALABRA_MIN || palabra > CENTINELA - 1){
            printf(" *** Valor fuera de rango. Use un entero entre %d y %d ***\n", PALABRA_MIN, CENTINELA - 1);
            continue;
        }
        memory[posicion] = palabra;
        posicion++;
    }
}
    
void ejecutarPrograma(void){
    
    int enEjecucion = 1;

    while (enEjecucion){
        instructionRegister = memory[instructionCounter];

        operationCode = instructionRegister / 100;
        operand = instructionRegister % 100;

        switch (operationCode){

            case OP_READ:
            printf("?");
            scanf("%d", &memory[operand]);
            instructionCounter++;
                break;

            case OP_WRITE:
            printf("%+05d\n", memory[operand]);
            instructionCounter++;
                break;

            case OP_LOAD:
                break;

            case OP_STORE:
                break;

            case OP_ADD:
                break;

            case OP_SUBSTRACT:
                break;

            case OP_DIVIDE:
                break;

            case OP_MULTIPLY:
                break;

            case OP_BRANCH:
                break;

            case OP_BRANCHENG:
                break;

            case OP_BRANCHZERO:
                break;

            case OP_HALT:
                printf("*** Termino la ejecucion de Simpletron ***\n");
                enEjecucion = 0;
                break;
        }
    }
    
}

void vaciadoMemoria(void){
    int i, j;

    printf("\nRegistros\n");
    printf("acumulador: %+05d\n", accumulator);
    printf("instructionCounter: %02d\n", instructionCounter);
    printf("instructionRegister: %+05d\n", instructionRegister);
    printf("operationCode: %02d\n", operationCode);
    printf("operand: %02d\n", operand);

    printf("memoria:\n");

    printf("  ");
    for(j = 0; j < 10; j++){
        printf("%6d", j);
    }
    printf("\n");

    for (i = 0; i < 10; i++)
    {
        printf("%2d", i);
        for (j = 0; j < 10; j++)
        {
            printf(" %+05d", memory[i * 10 + j]);
        }
        printf("\n");
    }
}