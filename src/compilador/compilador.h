/* ==========================================================
 *  compilador.h - Compilador de Simple a SML
 *  Autor: Xavier Hernandez Toledo
 *  Programacion Avanzada - Prof. Roberto Salazar
 * ========================================================== */

#ifndef COMPILADOR_H
#define COMPILADOR_H

#define TAM_MEMORIA 1000
#define TAM_LINEA 200
#define DIVISOR_OPERANDO 1000
#define CONSTANTE_MIN -99999
#define CONSTANTE_MAX 99998

#define OP_READ 10
#define OP_WRITE 11
#define OP_LOAD 20
#define OP_STORE 21
#define OP_ADD 30
#define OP_SUBTRACT 31
#define OP_DIVIDE 32
#define OP_MULTIPLY 33
#define OP_POW 35
#define OP_BRANCH 40
#define OP_BRANCHNEG 41
#define OP_BRANCHZERO 42
#define OP_HALT 43

int compilar(const char archivoEntrada[], const char archivoSalida[]);

#endif