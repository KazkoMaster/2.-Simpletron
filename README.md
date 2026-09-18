#Simulador de simpletron

Simulador en C de la computadora virtual  Simpletron, capaz de cargar y ejecutar programas escritos en SML.

Materia: Programación Avanzada
Profesor: Roberto Salazar
Alumno: Xavier Hernandez Toledo

## Descripcion

La simpletron es una computadora virtual de arquitectura von Neumann con:

- Memoria de **100 palabras**, simulada con el arreglo 'memory'
- Una palabra es un entero decimal **con signo de 4 digitos** (-9999 a 9999)
- Cinco registros: 'accumulator', 'instructionCounter','instructionRegister', 'operationCode' y 'operand'
- Doce operaciones en lenguaje SML

El simulador implementa el ciclo de **busqueda, decodificación y ejecución**:
trae la instruccion de memoria al instructionRegister, la separa en código de operación y operando mediante división entera y módulo, y la ejecuta en una estructura 'switch'

## Compilación

```
gcc -Wall -o Simpletron.exe Simpletron.c
```

## Ejecución

```
.\Simpletron.exe
```

El simulador solicita las instrucciones una por una, mostrando la posición de memorua seguida de un signo de interrogación. Se introduce **9999** para terminar la carga y comenzar la ejecución.

## Conjunto de instrucciones SML

Cada intrucción es una palabra de cuatro dígitos: los dos primeros son el código de operación (OP) y los dos ultimos la dirección de memoria. Por ejemplo, `+1007` es `READ` sobre la posición `07`.

| Opcode | Nombre | Efecto |
|:---:|---|---|
| 10 | `READ` | Lee una palabra del teclado y la guarda en `memory[ADDR]` |
| 11 | `WRITE` | Imprime `memory[ADDR]` |
| 20 | `LOAD` | `accumulator = memory[ADDR]` |
| 21 | `STORE` | `memory[ADDR] = accumulator` |
| 30 | `ADD` | `accumulator += memory[ADDR]` |
| 31 | `SUBTRACT` | `accumulator -= memory[ADDR]` |
| 32 | `DIVIDE` | `accumulator /= memory[ADDR]` |
| 33 | `MULTIPLY` | `accumulator *= memory[ADDR]` |
| 40 | `BRANCH` | `instructionCounter = ADDR` |
| 41 | `BRANCHNEG` | Si `accumulator < 0`, `instructionCounter = ADDR` |
| 42 | `BRANCHZERO` | Si `accumulator == 0`, `instructionCounter = ADDR` |
| 43 | `HALT` | Detiene la ejecución |

## Validaciones y manejo de errores

### Fase de carga

- Cada palabra debe estar en el intervalo **-9999 a +9998**. El limite superior es 9998 porque 9999 está reservado como centinela de fin de carga.
- Si el valor esta fuera de rango o no es un número, se vuelve a solicitar la **misma** posición hasta que la entrada sea válida.

### Fase de ejecución - errores fatales

Al detectarse cualquiera de estos, el simulador imprime un mensaje de error y un **vaciado de memoria completo**, y termina la ejecución:

| Error fatal | Cuándo ocurre |
|---|---|
| Intento de dividir entre cero | `DIVIDE` cuando `memory[ADDR]` vale 0 |
| Código de operación no válido | El OP no corresponde a ninguna de las doce operaciones |
| Desbordamiento del acumulador | Un resultado aritmético queda fuera de `[−9999, +9999]` |

Las operaciones aritmeticas calculan en una variable auxiliar y solo asignan al acumulador si el resultado cabe en una palabra, de modo que el acumulador nunca llega a contener un valor imposible para la maquina.

## Caso de prueba 1 - ejecutación correcta

Programa SML que lee dos números y muestra el mayor.

| Posición | Palabra | Instrucción |
|:---:|:---:|---|
| 00 | `+1009` | `READ A` |
| 01 | `+1010` | `READ B` |
| 02 | `+2009` | `LOAD A` |
| 03 | `+3110` | `SUBTRACT B` |
| 04 | `+4107` | `BRANCHNEG 07` |
| 05 | `+1109` | `WRITE A` |
| 06 | `+4300` | `HALT` |
| 07 | `+1110` | `WRITE B` |
| 08 | `+4300` | `HALT` |
| 09 | `+0000` | Variable A |
| 10 | `+0000` | Variable B |

Entradas: `25` y `17`. Salida esperada: `+0025`.

```
*** Bienvenido a Simpletron! ***
*** Introduzca su programa una instruccion ***
*** (o palabra de datos) a la vez. Yo indicare ***
*** el numero de posicion y una interrogacion (?) ***
*** Ustede tecleara entonces la palabra para esa ***
*** posicion. Escriba 9999 para dejar de ***
*** introducir su programa ***
00 ? 1009
01 ? 1010
02 ? 2009
03 ? 3110
04 ? 4107
05 ? 1109
06 ? 4300
07 ? 1110
08 ? 4300
09 ? 0000
10 ? 0000
11 ? 9999
*** Se termino de cargar el programa ***
*** Comienza la ejecucion del programa ***
? 25
? 17
+0025
*** Termino la ejecucion de Simpletron ***

Registros
acumulador: +0008
instructionCounter: 06
instructionRegister: +4300
operationCode: 43
operand: 00
memoria:
       0     1     2     3     4     5     6     7     8     9
 0 +1009 +1010 +2009 +3110 +4107 +1109 +4300 +1110 +4300 +0025
 1 +0017 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
 2 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
 3 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
 4 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
 5 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
 6 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
 7 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
 8 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
 9 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
```

## Caso de prueba 2 - error fatal (división entre 0)

| Posición | Palabra | Instrucción |
|:---:|:---:|---|
| 00 | `+1007` | `READ A` |
| 01 | `+1008` | `READ B` |
| 02 | `+2007` | `LOAD A` |
| 03 | `+3208` | `DIVIDE B` |
| 04 | `+2109` | `STORE C` |
| 05 | `+1109` | `WRITE C` |
| 06 | `+4300` | `HALT` |
| 07 | `+0000` | Variable A |
| 08 | `+0000` | Variable B |
| 09 | `+0000` | Variable C |

Entradas: `100` y `0`.

El `DIVIDE` de la posición 03 detecta que el divisor es cero, imprime el mensaje
de error fatal y el vaciado de memoria. El `instructionCounter` queda en `03`,
señalando la instrucción que provocó el error.

```
*** Bienvenido a Simpletron! ***
*** Introduzca su programa una instruccion ***
*** (o palabra de datos) a la vez. Yo indicare ***
*** el numero de posicion y una interrogacion (?) ***
*** Ustede tecleara entonces la palabra para esa ***
*** posicion. Escriba 9999 para dejar de ***
*** introducir su programa ***
00 ? 1007
01 ? 1008
02 ? 2007
03 ? 3208
04 ? 2109
05 ? 1109
06 ? 4300
07 ? 0000
08 ? 0000
09 ? 0000
10 ? 9999
*** Se termino de cargar el programa ***
*** Comienza la ejecucion del programa ***
? 100
? 0

*** Intento dividir entre cero ***
*** La ejecucion termino anormalmente ***

Registros
acumulador: +0100
instructionCounter: 03
instructionRegister: +3208
operationCode: 32
operand: 08
memoria:
       0     1     2     3     4     5     6     7     8     9
 0 +1007 +1008 +2007 +3208 +2109 +1109 +4300 +0100 +0000 +0000
 1 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
 2 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
 3 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
 4 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
 5 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
 6 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
 7 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
 8 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
 9 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
```

