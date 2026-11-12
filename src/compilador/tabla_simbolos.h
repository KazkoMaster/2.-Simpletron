#ifndef TABLA_SIMBOLOS_H
#define TABLA_SIMBOLOS_H

#define TAM_TABLA 1000

#define TIPO_LINEA 'L'
#define TIPO_VARIABLE 'V'
#define TIPO_CONSTANTE 'C'
#define TIPO_TEMPORAL 'T'

typedef struct{
    int simbolo;
    char tipo;
    int ubicacion;
} EntradaTabla;

void inicializarTabla(void);
int buscarSimbolo(int simbolo, char tipo);
int agregarSimbolo(int simbolo, char tipo, int ubicacion);
int buscarPorUbicacion(int ubicacion, EntradaTabla *entrada);
void reubicarTabla(int limite, int desplazamiento);
void imprimirTabla(void);

#endif