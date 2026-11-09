# Simulador de Simpletron
 
Simulador en C de la computadora virtual Simpletron, capaz de cargar y ejecutar programas escritos en SML.
 
- Materia: Programación Avanzada
- Profesor: Roberto Salazar
- Alumno: Xavier Hernandez Toledo
## Descripción
 
La Simpletron es una computadora virtual de arquitectura von Neumann con:
 
- Memoria de **1000 palabras** (direcciones `000` a `999`), simulada con el arreglo `memory`
- Una palabra es un entero decimal **con signo de 5 dígitos** (−99999 a +99999)
- Registros: `accumulator`, `instructionCounter`, `instructionRegister`, `operationCode` y `operand`
- Una memoria y un acumulador adicionales para punto flotante: `memoriaFlotante` y `acumuladorFlotante`
- 25 instrucciones SML

El simulador implementa el ciclo de **busqueda, decodificacion y ejecucion

## Formato de instrucción

Cada instrucción tiene la forma `CC DDD`:

- `CC`: Codigo de operació, 2 digitos
-`DDD`: dirección de memoria, 3 digitos

Por ejemplo, `10500` es `READ` sobre la dirección de `500`. La decodificación es:

```c
operationCode = instructionRegister / 1000;
operand       = instructionRegister % 1000;
```
 
## Compilación
 
```
gcc -Wall -o Simpletron.exe Simpletron.c
```
 
Compila sin errores ni advertencias con `-Wall`.
 
## Ejecución
 
```
.\Simpletron.exe
```
 
El simulador tiene 2 modos de carga:

| Situación | Comportamiento |
| --- | --- |
| Existe `programa.simp` en la carpeta | Carga el programa automaticamente desde el archivo |
| No existe `programa.simp` | Modo interactivo: pide cada palabra mostrando la posicion y `?` |

En ambos modos, los datos de `READ`, `READ_STR` Y `READ_F` se piden por teclado durante la ejecución.

## Formato de `Programa.simp`

- Una palabra por linea, en el orden en que se cargan a memoria (la primera va a la `000`)
- `99999` marca el fin del programa. Si no está, se carga hasta el final del archivo
- Las lineas en blanco se ignoran
- Si una linea no es un entero o está fuera de rango, el simulador indica ek número de linea y **no ejecuta el programa**

Para volver al modo interactivo, solo se ocupa borrar o renombrar `Promgrama.simp`.
 
## Conjunto de instrucciones SML
 
Cada instrucción es una palabra de cuatro dígitos: los dos primeros son el código de operación (OP) y los dos últimos la dirección de memoria (DDD). Por ejemplo, `+1007` es `READ` sobre la posición `07`.
 
| Opcode | Nombre | Efecto |
|:---:|---|---|
| 10 | `READ` | Lee una palabra del teclado y la guarda en `memory[DDD]` |
| 11 | `WRITE` | Imprime `memory[DDD]` |
| 12 | `NEWLINE` | Imprime un salto de linea (el operando no se usa) |
| 13 | `READ_STR` | Lee una cadena y la guarda a partir de `DDD` |
| 14 | `WRITE_STR` | Imprime la cadena guardada a partir de `DDD` |
| 15 | `READ_F` | Lee un flotante y lo guarda en `memoriaFLotante[DDD]` |
| 16 | `WRITE_F` | Imprime `memoriaFlotante[DDD]` con 4 decimales | 
| 20 | `LOAD` | `accumulator = memory[DDD]` |
| 21 | `STORE` | `memory[DDD] = accumulator` |
| 22 | `LOAD_F` | `acumuladorFlotante = memoriaFLotante[DDD]` | 
| 23 | `STORE_F` | `memoriaFLotante[DDD] = acumuladorFLotante` | 
| 30 | `ADD` | `accumulator += memory[DDD]` |
| 31 | `SUBTRACT` | `accumulator -= memory[DDD]` |
| 32 | `DIVIDE` | `accumulator /= memory[ADD]` |
| 33 | `MULTIPLY` | `accumulator *= memory[DDD]` |
| 34 | `MOD` | `accumulator = accumulator % memory[DDD]` |
| 35 | `POW` | `accumulator = accumulator ^ memory[DDD]` |
| 40 | `BRANCH` | `instructionCounter = DDD` |
| 41 | `BRANCHNEG` | Si `accumulator < 0`, `instructionCounter = DDD` |
| 42 | `BRANCHZERO` | Si `accumulator == 0`, `instructionCounter = DDD` |
| 43 | `HALT` | Detiene la ejecución |
| 50 | `ADD_F` | `acumuladorFlotante += memoriaFLotante[DDD]` |
| 51 | `SUBTRACT_F` | `acumuladorFlotante -= memoriaFLotante[DDD]` |
| 52 | `DIVIDE_F` | `acumuladorFlotante /= memoriaFLotante[DDD]` | 
| 53 | `MULTIPLY_F` | `acumuladorFlotante *= memoriaFLotante[DDD]` |

### Reglas de `MOD` y `POW`

- `MOD`: si el divisor es 0 hay error fatal. El signo del residuo es el del dividendo, como en C: −17 mod 5 = −2.
- `POW`: el exponente debe ser ≥ 0; un exponente negativo es error fatal. Cualquier base elevada a 0 da 1, incluido 0^0. Se calcula por multiplicación repetida y se detiene con error fatal en cuanto el resultado sale de ±99999.
- Las operaciones aritméticas enteras calculan en una variable `long long` y solo asignan al acumulador si el resultado cabe en una palabra. Así se evita el desbordamiento del tipo `int` de C, por ejemplo en 65536 × 65536.

## Representaciones de cadenas

Una cadena se guarda a partir de una dirección base, con un caracter por palabra:

- **Primera palabra:** la lingitud en la primera mitas y `000` en la segunda
- **Palabras siguientes:** la posicion de carácter (01, 02, ...) en la primera mitas y su codigo ASCII (3 digitos) en la segunda

Ejemplo: `"Mundo"` guardada a partir de la dirección `040`:

| 040 | 041 | 042 | 043 | 044 | 045 |
|---|---|---|---|---|---|
| `05 000` | `01 077` | `02 117` | `03 110` | `04 100` | `05 111` |
| longitud 5 | `M` | `u` | `n` | `d` | `o` |

Reglas: 

- Longitud maxima: **99 caracteres**, porque la longitud ocupa 2 digitos
- Si la cadena no cabe en memoria (base + longitud > 999), hay error fatal
- `WRITE_STR` valida la cadena completa **antes** de imprimir: la palabra base debe terminar en `000`, cada caracter debe llevar su posición correcta y un codigo entre 0 y 255, si algo no cumpl, hay error fatal y no se imprime nada
- `WRITE_STR` no agrega salto de linea, se usa `NEWLINE` (`12000`) despues

## Representación de punto flotante

- Los flotantes se guarancomo `double` de C (IEEE 754) en el arreglo `memoriaFlotante`, quetiene las mismas 1000 direcciones que la memoria entera
- Tienen su propio acumulador, `acumuladorFlotante`. La parte entera del simulador no cambia
- Rango permitido: **±1 000 000 000 000** (`FLOTANTE_MAX = 1.0e12`), un valor leido fuera de rango se rechaza y se vuelve a pedir; un resultadofuera de rango es error fatal
- `WRITE_F` imprime 4 decimales
- `READ_F` acepta notación decimal con punto (`3.5`) y cientifica (`1e3`), rechaza textos, comas (`3,5`) y texto pegado al numero ('3.5x')
- El vaciado de memoria muestra `acumulador flotante` y, al final, solo las posiciones de la memoria flotante distintas de cero
 
## Validaciones y manejo de errores
 
### Fase de carga
 
- Cada palabra debe estar entre **−99999 y +99998**. El 99999 está reservado como centinela de fin de carga
- **Modo interactivo:** si el valor esta fuera de rango o no es un numero, vuelve a pedir la **misma** posición
- **Modo archivo:** indica la línea con error y no ejecuta el programa

### Fase de ejecución — lectura
 
- `READ` acepta enteros entre −99999 y +99999 y vuelve a pedir el dato si es invalido
- Si la entrada se termina (fin de archivo), hay error fatal en lugar de quedarse en un ciclo infinito

### Fase de ejecución — errores fatales
 
Al detectarse cualquiera de estos, el simulador imprime un mensaje de error y un **vaciado de memoria completo**, y termina la ejecución:
 
| Error fatal | Cuándo ocurre |
|---|---|
| Intento de dividir entre cero | `DIVIDE` cuando `memory[DDD]` vale 0 o `DIVIDE_F` con divisor 0 |
| Intento de calcular el residuo entre 0 | `MOD` con divisor 0 |
| Exponente negativo no permitido | `POW` con exponente < 0 | 
| Desbordamiento del acumulador | Resultado entero fuera de ±99999 | 
| Desboradmiento del acumulador flotante | Resultado flotante fuera de ±1e12 |
| Código de operación no válido | El codigo no corresponde a ninguna instrucción |
| Contador fuera de la memoria | El `instructionCounter` sale del rango `000`–`999` |
| Dirección de la memoria fuera de rango | El operando no esta entre `000` y `999` | 
| Se termino la entrada de datos | Un `READ`, `RED_STR` y `READ_F` llega al fin de la entrada | 
| La cadena no cabe en la memoria | `READ_STR` cuando base + longitud > 999 |
| No hay una cadena valida en esa direccion | `WRITE_STR` sobre una palabra que no es encabezado de cadena | 
| La cadena se sale de la memoria | `WRITE_STR` cuando la longitud indicada pasa de la dirección 999 | 
| La cadena tiene un caracter con formato invalido | `WRITE_STR` encuentra una palabra con posición o codigo incorrecto |
 
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

## Casos de prueba mejoras

Todos los casos usan `programa.simp`, salvo los de modo interactivo. Para cada caso se muestra el contenido del archivo, los datos que se escriben al ejecutar y el resultado. Por espacio, de cada salida se muestran solo las líneas relevantes; el vaciado completo tiene 100 filas.

### Resumen

| # | Caso | Mejora | Resultado |
|:---:|---|---|---|
| 1 | Mayor de dos números | Base | Imprime `+00025` |
| 2 | División entre cero | Base | Error fatal, contador en `003` |
| 3 | Código de operación no válido | Base | Error fatal, contador en `001` |
| 4 | Desbordamiento del acumulador | Base | Error fatal, contador en `003` |
| 5 | Validación de la carga interactiva | Base / 1 | Vuelve a pedir la posición `000` |
| 6 | Direcciones mayores a 99 | 2 | Imprime `+00220` usando 120, 500 y 999 |
| 7 | Salto a memoria vacía | 2 | Error fatal, contador en `500` |
| 8 | Archivo correcto | 1 | Carga 7 palabras y ejecuta |
| 9 | Archivo inexistente | 1 | Modo interactivo |
| 10 | Archivo con línea inválida | 1 | Indica la línea 3 y no ejecuta |
| 11–16 | Residuo (6 pruebas) | 3 | Ver tabla |
| 17–24 | Potencia (8 pruebas) | 4 | Ver tabla |
| 25 | Multiplicación sin desbordar `int` | 3 / 4 | 65536 × 65536 marca desbordamiento |
| 26–27 | Salto de línea (2 pruebas) | 5 | Ver salidas |
| 28–33 | Entrada de cadenas (6 pruebas) | 6 | Ver tabla |
| 34–38 | Salida de cadenas (5 pruebas) | 7 | Ver tabla |
| 39–46 | Punto flotante (8 pruebas) | 8 | Ver tabla |

---

### Casos 1 - 4: funcionamiento base

Son los casos de la versión original adaptados a los 5 digitos

**Caso 1: mayor de dos números.** Si A − B es negativo, `BRANCHNEG` salta a imprimir B.

| Posición | Palabra | Instrucción |
|:---:|:---:|---|
| 000 | `10100` | `READ A` |
| 001 | `10101` | `READ B` |
| 002 | `20100` | `LOAD A` |
| 003 | `31101` | `SUBTRACT B` |
| 004 | `41007` | `BRANCHNEG 007` |
| 005 | `11100` | `WRITE A` |
| 006 | `43000` | `HALT` |
| 007 | `11101` | `WRITE B` |
| 008 | `43000` | `HALT` |

```
10100
10101
20100
31101
41007
11100
43000
11101
43000
99999
```

Datos: `25` y `17`.

```
*** Se termino de cargar el programa (9 palabras) ***
*** Comienza la ejecucion del programa ***
? ? +00025
*** Termino la ejecucion de Simpletron ***

Registros:
acumulador:          +00008
instructionCounter:     006
instructionRegister: +43000
```
**Caso 2: división entre cero.**

```
10100
10101
20100
32101
21102
11102
43000
99999
```

Datos: `100` y `0`.

```
*** Intento de dividir entre cero ***
*** La ejecucion de Simpletron termino anormalmente ***

Registros:
acumulador:          +00100
instructionCounter:     003
instructionRegister: +32101
```

**Caso 3: código de operación no válido.** La palabra `99000` tiene código 99, que no existe.

```
20005
99000
43000
0
0
7
99999
```

```
*** Se intento ejecutar un codigo de operacion no valido ***
*** La ejecucion de Simpletron termino anormalmente ***

Registros:
acumulador:          +00007
instructionCounter:     001
instructionRegister: +99000
operationCode:           99
```

**Caso 4: desbordamiento del acumulador.** 99999 + 1 no cabe en una palabra.

```
10100
10101
20100
30101
21102
11102
43000
99999
```

Datos: `99999` y `1`.

```
*** Desbordamiento del acumulador ***
*** La ejecucion de Simpletron termino anormalmente ***

Registros:
acumulador:          +99999
instructionCounter:     003
instructionRegister: +30101
```

El acumulador conserva su último valor válido, porque la suma se calcula en una variable auxiliar.

---