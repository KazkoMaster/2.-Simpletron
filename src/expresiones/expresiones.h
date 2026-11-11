#ifndef EXPRESIONES_H
#define EXPRESIONES_H

#define TAM_EXPRESION 200
#define TAM_POSTFIJA (2 * TAM_EXPRESION)

int profundidad(const char expresion[]);
int verificarAgrupacion(const char expresion[]);
int infijaAPostfija(const char infija[], char postfija[]);

#endif
