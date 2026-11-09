; mayor.sml - Imprime el mayor de dos numeros
; Codigo: 000-008   Datos: 100(A), 101(B)

10100 ; 000 READ A
10101 ; 001 READ B
20100 ; 002 LOAD A
31101 ; 003 SUBTRACT B      acc = A - B
41007 ; 004 BRANCHNEG 007   si A < B salta a imprimir BRANCHNEG
11100 ; 005 WRITE A 
43000 ; 006 HALT
11101 ; 007 WRITE B
43000 ; 008 HALT
99999 ; fin del programa