#include <stdio.h>
#include "compilador.h"

int main(int argc, char *argv[]){

    if(argc != 3){
        printf("*** Uso: %s <programa.simple> <programa.sml> ***\n", argv[0]);
        return 1;
    }

    if(!compilar(argv[1], argv[2])){
        return 1;
    }

    return 0;
}