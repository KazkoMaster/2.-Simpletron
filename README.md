# Simulador de Simpletron
 
Simulador en C de la computadora virtual Simpletron, capaz de cargar y ejecutar programas escritos en SML.
 
- Materia: Programación Avanzada
- Profesor: Roberto Salazar
- Alumno: Xavier Hernandez Toledo
## Descripción
 
La Simpletron es una computadora virtual de arquitectura von Neumann con:
 
- Memoria de **100 palabras**, simulada con el arreglo `memory`
- Una palabra es un entero decimal **con signo de 4 dígitos** (−9999 a +9999)
- Cinco registros: `accumulator`, `instructionCounter`, `instructionRegister`, `operationCode` y `operand`
- Doce operaciones en lenguaje SML
El simulador implementa el ciclo de **búsqueda, decodificación y ejecución**:
trae la instrucción de memoria al `instructionRegister`, la separa en código de operación y operando mediante división entera y módulo, y la ejecuta en una estructura `switch`.
 
## Compilación
 
```
gcc -Wall -o Simpletron.exe Simpletron.c
```
 
Compila sin errores ni advertencias con `-Wall`.
 
## Ejecución
 
```
.\Simpletron.exe
```
 
El simulador solicita las instrucciones una por una, mostrando la posición de memoria seguida de un signo de interrogación. Se introduce **9999** para terminar la carga y comenzar la ejecución.
 
## Conjunto de instrucciones SML
 
Cada instrucción es una palabra de cuatro dígitos: los dos primeros son el código de operación (OP) y los dos últimos la dirección de memoria (ADDR). Por ejemplo, `+1007` es `READ` sobre la posición `07`.
 
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
 
- Cada palabra debe estar en el intervalo **−9999 a +9998**. El límite superior es 9998 porque 9999 está reservado como centinela de fin de carga.
- Si el valor está fuera de rango o no es un número, se vuelve a solicitar la **misma** posición hasta que la entrada sea válida.
### Fase de ejecución — lectura
 
- Cada `READ` valida que el valor esté entre **−9999 y +9999**; si no, vuelve a solicitarlo.
### Fase de ejecución — errores fatales
 
Al detectarse cualquiera de estos, el simulador imprime un mensaje de error y un **vaciado de memoria completo**, y termina la ejecución:
 
| Error fatal | Cuándo ocurre |
|---|---|
| Intento de dividir entre cero | `DIVIDE` cuando `memory[ADDR]` vale 0 |
| Código de operación no válido | El OP no corresponde a ninguna de las doce operaciones |
| Desbordamiento del acumulador | Un resultado aritmético queda fuera de `[−9999, +9999]` |
| Contador fuera de la memoria | El `instructionCounter` sale del rango `00`–`99` |
 
Las operaciones aritméticas calculan en una variable auxiliar y solo asignan al acumulador si el resultado cabe en una palabra, de modo que el acumulador nunca llega a contener un valor imposible para la máquina.
 
## Casos de prueba
 
| # | Caso | Tipo | Resultado |
|:---:|---|---|---|
| 1 | Mayor de dos números | Ejecución correcta | Imprime `+0025` |
| 2 | División entre cero | Error fatal | Vaciado con contador en `03` |
| 3 | Código de operación no válido | Error fatal | Vaciado con contador en `01` |
| 4 | Desbordamiento del acumulador | Error fatal | Vaciado con contador en `03` |
| 5 | Validación de la carga | Rechazo de entradas | Vuelve a pedir la posición `00` |
 
Las salidas se obtuvieron pasando la entrada completa por la entrada estándar. Por eso los valores aparecen en bloque después del primer prompt y los prompts siguientes se muestran seguidos en una misma línea.
 
### Caso 1 — Mayor de dos números
 
Lee A y B e imprime el mayor. Ejercita la transferencia de control condicional: si A − B es negativo, `BRANCHNEG` salta a imprimir B.
 
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
 
Entrada:
 
```
1009
1010
2009
3110
4107
1109
4300
1110
4300
0000
0000
9999
25
17
```
 
Resultado: imprime `+0025` y termina en el `HALT` de la posición 06. El acumulador queda en `+0008`, resultado de 25 − 17.
 
```
*** Bienvenido a Simpletron! ***
*** Introduzca su programa una instruccion ***
*** (o palabra de datos) a la vez. Yo indicare ***
*** el numero de posicion y una interrogacion (?). ***
*** Usted tecleara entonces la palabra para esa ***
*** posicion. Escriba 9999 para dejar de ***
*** introducir su programa. ***
00 ? 1009
1010
2009
3110
4107
1109
4300
1110
4300
0000
0000
9999
25
17
01 ? 02 ? 03 ? 04 ? 05 ? 06 ? 07 ? 08 ? 09 ? 10 ? 11 ? *** Se termino de cargar el programa ***
*** Comienza la ejecucion del programa ***
? ? +0025
*** Termino la ejecucion de Simpletron ***
 
Registros:
acumulador:          +0008
instructionCounter:     06
instructionRegister: +4300
operationCode:          43
operand:                00
 
MEMORIA
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
 
### Caso 2 — Error fatal: división entre cero
 
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
 
Entrada:
 
```
1007
1008
2007
3208
2109
1109
4300
0000
0000
0000
9999
100
0
```
 
Resultado: el `DIVIDE` de la posición 03 detecta que el divisor es cero, imprime el mensaje de error y el vaciado. El contador queda en `03` y el `instructionRegister` en `+3208`, señalando la instrucción que provocó el error. El acumulador conserva `+0100`.
 
```
*** Bienvenido a Simpletron! ***
*** Introduzca su programa una instruccion ***
*** (o palabra de datos) a la vez. Yo indicare ***
*** el numero de posicion y una interrogacion (?). ***
*** Usted tecleara entonces la palabra para esa ***
*** posicion. Escriba 9999 para dejar de ***
*** introducir su programa. ***
00 ? 1007
1008
2007
3208
2109
1109
4300
0000
0000
0000
9999
100
0
01 ? 02 ? 03 ? 04 ? 05 ? 06 ? 07 ? 08 ? 09 ? 10 ? *** Se termino de cargar el programa ***
*** Comienza la ejecucion del programa ***
? ? 
*** Intento de dividir entre cero ***
*** La ejecucion de Simpletron termino anormalmente ***
 
Registros:
acumulador:          +0100
instructionCounter:     03
instructionRegister: +3208
operationCode:          32
operand:                08
 
MEMORIA
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
 
### Caso 3 — Error fatal: código de operación no válido
 
| Posición | Palabra | Instrucción |
|:---:|:---:|---|
| 00 | `+2005` | `LOAD 05` |
| 01 | `+9900` | Código 99, no existe en SML |
| 02 | `+4300` | `HALT`, nunca se alcanza |
| 03 | `+0000` | Sin uso |
| 04 | `+0000` | Sin uso |
| 05 | `+0007` | Dato |
 
Entrada:
 
```
2005
9900
4300
0000
0000
0007
9999
```
 
Resultado: el `LOAD` se ejecuta normalmente; en la posición 01 el código 99 cae en el caso `default` del `switch`, que lo reporta como error fatal. El contador queda en `01`, `operationCode` en `99` y el acumulador en `+0007`.
 
```
*** Bienvenido a Simpletron! ***
*** Introduzca su programa una instruccion ***
*** (o palabra de datos) a la vez. Yo indicare ***
*** el numero de posicion y una interrogacion (?). ***
*** Usted tecleara entonces la palabra para esa ***
*** posicion. Escriba 9999 para dejar de ***
*** introducir su programa. ***
00 ? 2005
9900
4300
0000
0000
0007
9999
01 ? 02 ? 03 ? 04 ? 05 ? 06 ? *** Se termino de cargar el programa ***
*** Comienza la ejecucion del programa ***
 
*** Se intento ejecutar un codigo de operacion no valido ***
*** La ejecucion de Simpletron termino anormalmente ***
 
Registros:
acumulador:          +0007
instructionCounter:     01
instructionRegister: +9900
operationCode:          99
operand:                00
 
MEMORIA
       0     1     2     3     4     5     6     7     8     9
 0 +2005 +9900 +4300 +0000 +0000 +0007 +0000 +0000 +0000 +0000
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
 
### Caso 4 — Error fatal: desbordamiento del acumulador
 
Suma dos números cuyo resultado, 9999 + 9999 = 19998, no cabe en una palabra.
 
| Posición | Palabra | Instrucción |
|:---:|:---:|---|
| 00 | `+1007` | `READ A` |
| 01 | `+1008` | `READ B` |
| 02 | `+2007` | `LOAD A` |
| 03 | `+3008` | `ADD B` |
| 04 | `+2109` | `STORE C` |
| 05 | `+1109` | `WRITE C` |
| 06 | `+4300` | `HALT` |
| 07 | `+0000` | Variable A |
| 08 | `+0000` | Variable B |
| 09 | `+0000` | Variable C |
 
Entrada:
 
```
1007
1008
2007
3008
2109
1109
4300
0000
0000
0000
9999
9999
9999
```
 
Resultado: el `ADD` de la posición 03 detecta el desbordamiento. El contador queda en `03` y el acumulador en `+9999`, su último valor válido: la suma se calcula en una variable auxiliar y solo se asigna al acumulador si cabe en una palabra.
 
```
*** Bienvenido a Simpletron! ***
*** Introduzca su programa una instruccion ***
*** (o palabra de datos) a la vez. Yo indicare ***
*** el numero de posicion y una interrogacion (?). ***
*** Usted tecleara entonces la palabra para esa ***
*** posicion. Escriba 9999 para dejar de ***
*** introducir su programa. ***
00 ? 1007
1008
2007
3008
2109
1109
4300
0000
0000
0000
9999
9999
9999
01 ? 02 ? 03 ? 04 ? 05 ? 06 ? 07 ? 08 ? 09 ? 10 ? *** Se termino de cargar el programa ***
*** Comienza la ejecucion del programa ***
? ? 
*** Desbordamiento del acumulador ***
*** La ejecucion de Simpletron termino anormalmente ***
 
Registros:
acumulador:          +9999
instructionCounter:     03
instructionRegister: +3008
operationCode:          30
operand:                08
 
MEMORIA
       0     1     2     3     4     5     6     7     8     9
 0 +1007 +1008 +2007 +3008 +2109 +1109 +4300 +9999 +9999 +0000
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
 
### Caso 5 — Validación de la carga
 
Se introducen tres valores inválidos en la posición 00 antes de uno válido.
 
Entrada:
 
```
99999
abc
-10000
4300
9999
```
 
Resultado: rechaza `99999` y `-10000` por estar fuera del intervalo −9999 a +9998, y `abc` por no ser un número entero. En los tres casos vuelve a solicitar la posición `00` sin avanzar. Después carga `+4300` y ejecuta el `HALT`, con el contador en `00`.
 
```
*** Bienvenido a Simpletron! ***
*** Introduzca su programa una instruccion ***
*** (o palabra de datos) a la vez. Yo indicare ***
*** el numero de posicion y una interrogacion (?). ***
*** Usted tecleara entonces la palabra para esa ***
*** posicion. Escriba 9999 para dejar de ***
*** introducir su programa. ***
00 ? 99999
abc
-10000
4300
9999
*** Valor fuera de rango. Use un entero entre -9999 y 9998. ***
00 ? *** Entrada invalida. Escriba un numero entero. ***
00 ? *** Valor fuera de rango. Use un entero entre -9999 y 9998. ***
00 ? 01 ? *** Se termino de cargar el programa ***
*** Comienza la ejecucion del programa ***
*** Termino la ejecucion de Simpletron ***
 
Registros:
acumulador:          +0000
instructionCounter:     00
instructionRegister: +4300
operationCode:          43
operand:                00
 
MEMORIA
       0     1     2     3     4     5     6     7     8     9
 0 +4300 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000 +0000
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