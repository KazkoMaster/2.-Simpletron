/* ==========================================================
 *  Simpletron.c - Simulador de la computadora Simpletron
 *  Autor: Xavier Hernandez Toledo
 *  Programacion Avanzada - Prof. Roberto Salazar
 * ========================================================== */

#include <stdio.h>

#define TAM_MEMORIA 1000
#define PALABRA_MIN -99999
#define PALABRA_MAX 99999
#define CENTINELA 99999
#define DIVISOR_OPERANDO 1000

#define ARCHIVO_PROGRAMA "programa.simp"
#define TAM_LINEA 100

#define OP_READ 10
#define OP_WRITE 11
#define OP_LOAD 20
#define OP_STORE 21
#define OP_ADD 30
#define OP_SUBTRACT 31
#define OP_DIVIDE 32
#define OP_MULTIPLY 33
#define OP_BRANCH 40
#define OP_BRANCHNEG 41
#define OP_BRANCHZERO 42
#define OP_HALT 43

int memory[TAM_MEMORIA];
int accumulator;
int instructionCounter;
int instructionRegister;
int operationCode;
int operand;
int direccionValida(int direccion);

void mostrarBienvenida(void);
void inicializar(void);
int cargarPrograma(FILE *fuente, int interactivo);
int leerDato(int *valor);
void ejecutarPrograma(void);
void vaciadoMemoria(void);
void errorFatal(char mensaje[]);

int main(void){

    FILE *archivo;
    int cargaCorrecta;

    inicializar();

    archivo = fopen(ARCHIVO_PROGRAMA, "r");

    if (archivo != NULL){
        printf("*** Bienvenido a Simpletron! ***\n");
        printf("*** Cargando programa desde %s***\n", ARCHIVO_PROGRAMA);
        cargaCorrecta = cargarPrograma(archivo, 0);
        fclose(archivo);
    } else{
        mostrarBienvenida();
        cargaCorrecta =  cargarPrograma(stdin, 1);
    }

    if(!cargaCorrecta){
        printf("*** La carga fallo. El programa no se ejecutara. ***\n");
        return 1;
    }

    ejecutarPrograma();
    vaciadoMemoria();

    return 0;

}

void mostrarBienvenida(void){

    printf("*** Bienvenido a Simpletron! ***\n");
    printf("*** Introduzca su programa una instruccion ***\n");
    printf("*** (o palabra de datos) a la vez. Yo indicare ***\n");
    printf("*** el numero de posicion y una interrogacion (?). ***\n");
    printf("*** Usted tecleara entonces la palabra para esa ***\n");
    printf("*** posicion. Escriba 99999 para dejar de ***\n");
    printf("*** introducir su programa. ***\n");

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

int cargarPrograma(FILE *fuente, int interactivo){

    char linea[TAM_LINEA];
    char sobrante;
    int posicion = 0;
    int numeroLinea = 0;
    int palabra;
    int leidos;

    while(posicion < TAM_MEMORIA){

        if(interactivo){
            printf("%03d ? ", posicion);
        }

        if(fgets(linea, TAM_LINEA, fuente)== NULL){
            break;
        }

        numeroLinea++;

        leidos = sscanf(linea, "%d %c", &palabra, &sobrante);

        if(leidos == EOF){
            continue;
        }

        if(leidos == 1 && palabra == CENTINELA){
            break;
        }

        if(leidos != 1 || palabra < PALABRA_MIN || palabra > CENTINELA - 1){

            if(!interactivo){
                printf("*** Error en %s, linea %d ***\n", ARCHIVO_PROGRAMA, numeroLinea);
            }

            if(leidos != 1){
                printf("*** Entrada invalida. Escriba un numero entero. ***\n");
            } else{
                printf("*** Valor fuera de rango. Use un entero entre %d y %d. ***\n",  PALABRA_MIN, CENTINELA - 1);
            }
            if(interactivo){
                continue;
            }

            return 0;
        }

        memory[posicion] = palabra;
        posicion++;
    }

    printf("*** Se termino de cargar el programa (%d palabras) ***\n", posicion);
    printf("*** Comienza la ejecucion del programa ***\n");

    return 1;
}

int leerDato(int *valor){

    int c;
    int leidos;

    while (1){
        printf("? ");
        leidos = scanf("%d", valor);

        if(leidos == EOF){
            return 0;
        }

        if(leidos != 1){
            printf("*** Entrada invalida. Escriba un numero entero. ***\n");
            while((c = getchar()) != '\n' && c != EOF){
                
            }
            continue;
        }

        if(*valor < PALABRA_MIN || *valor > PALABRA_MAX){
            printf("*** Valor fuera de rango. Use un entero entre %d y %d. ***\n", PALABRA_MIN, PALABRA_MAX);
            continue;
        }

        return 1;
    }
}
    
void ejecutarPrograma(void){
    
    int valor;
    int enEjecucion = 1;
    int resultado;

    while (enEjecucion){

         if (instructionCounter < 0 || instructionCounter >= TAM_MEMORIA) {
            errorFatal("El contador de instrucciones salio de la memoria");
            enEjecucion = 0;
            break;
        }

        instructionRegister = memory[instructionCounter];

        operationCode = instructionRegister / DIVISOR_OPERANDO;
        operand = instructionRegister % DIVISOR_OPERANDO;

        if (!direccionValida(operand)) {
            errorFatal("Direccion de memoria fuera de rango");
            enEjecucion = 0;
            break;
        }

        switch (operationCode){

            case OP_READ:
                if(!leerDato(&valor)){
                    errorFatal("Se termino la entrada de datos");
                    enEjecucion = 0;
                    break;
                }
                memory[operand] = valor;
                instructionCounter++;
                break;

            case OP_WRITE:
            printf("%+06d\n", memory[operand]);
            instructionCounter++;
                break;

            case OP_LOAD:
                accumulator = memory[operand];
                instructionCounter++;
                break;

            case OP_STORE:
            memory[operand] = accumulator;
            instructionCounter++;
                break;

            case OP_ADD:
                resultado = accumulator + memory[operand];
                if (resultado < PALABRA_MIN || resultado > PALABRA_MAX){
                    errorFatal("Desbordamiento del acumulador");
                    enEjecucion = 0;
                    break;
                }
                accumulator = resultado;
                instructionCounter++;
                break;

            case OP_SUBTRACT:
                resultado = accumulator - memory[operand];
                if (resultado < PALABRA_MIN || resultado > PALABRA_MAX){
                    errorFatal("Desbordamiento del acumulador");
                    enEjecucion = 0;
                    break;
                }
                accumulator = resultado;
                instructionCounter++;
                break;

            case OP_DIVIDE:
            if (memory[operand] == 0){
                errorFatal("Intento de dividir entre cero");
                enEjecucion = 0;
                break;
            } 
            resultado = accumulator / memory[operand];
            if (resultado < PALABRA_MIN || resultado > PALABRA_MAX){
                errorFatal("Desbordamiento del acumulador");
                enEjecucion = 0;
                break;
                }
            accumulator = resultado;
            instructionCounter++;
            break;

            case OP_MULTIPLY:
            resultado = accumulator * memory[operand];
                if (resultado < PALABRA_MIN || resultado > PALABRA_MAX){
                    errorFatal("Desbordamiento del acumulador");
                    enEjecucion = 0;
                    break;
                }
                accumulator = resultado;
                instructionCounter++;
                break;

            case OP_BRANCH:
                instructionCounter = operand;
                break;

            case OP_BRANCHNEG:
                if (accumulator < 0){
                    instructionCounter = operand;
                }else {
                    instructionCounter++;
                }
                break;

            case OP_BRANCHZERO:
                if (accumulator == 0){
                    instructionCounter = operand;
                }else {
                    instructionCounter++;
                }
                break;

            case OP_HALT:
                printf("*** Termino la ejecucion de Simpletron ***\n");
                enEjecucion = 0;
                break;

            default:
                errorFatal("Se intento ejecutar un codigo de operacion no valido");
                enEjecucion = 0;
                break;
        }
    }
    
}

void vaciadoMemoria(void){
    int i, j;

    printf("\nRegistros:\n");
    printf("acumulador:          %+06d\n", accumulator);
    printf("instructionCounter:     %03d\n", instructionCounter);
    printf("instructionRegister: %+06d\n", instructionRegister);
    printf("operationCode:           %02d\n", operationCode);
    printf("operand:                %03d\n", operand);

    printf("\nMEMORIA\n");

    printf("   ");
    for(j = 0; j < 10; j++){
        printf("%7d", j);
    }
    printf("\n");

    for (i = 0; i < TAM_MEMORIA / 10; i++)
    {
        printf("%3d", i * 10);
        for (j = 0; j < 10; j++)
        {
            printf(" %+06d", memory[i * 10 + j]);
        }
        printf("\n");
    }
}

void errorFatal(char mensaje[]){
    printf("\n*** %s ***\n", mensaje);
    printf("*** La ejecucion de Simpletron termino anormalmente ***\n");
}

int direccionValida(int direccion){
    return direccion >= 0 && direccion < TAM_MEMORIA;
}