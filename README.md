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