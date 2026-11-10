; promedio.sml - Repeticion
; LEE N y luego N numeros; imprime su suma y su promedio entero.
; Si N es cero o negativo termina sin leer nada
; Codigo: 000-020   Datos: 021-026

10021 ; 000 READ N
20021 ; 001 LOAD N
41020 ; 002 BRANCHNEG 020   si N < 0 termina
42020 ; 003 BRANCHZERO 020   si N = 0 termina
21024 ; 004 STORE CONT   CONT = N
20024 ; 005 LOAD CONT    <- inicio del ciclo
42015 ; 006 BRANCHZERO 015 si CONT = 0 sale del ciclo
10022 ; 007 READ X
20023 ; 008 LOAD SUMA
30022 ; 009 ADD X
21023 ; 010 STORE SUMA
20024 ; 011 LOAD CONT   CONT = CONT - 1
31025 ; 012 SUBTRACT UNO
21024 ; 013 STORE CONT
40005 ; 014 BRANCH 005   regresa al inicio del ciclo
11023 ; 015 WRITE SUMA
20023 ; 016 LOAD SUMA
32021 ; 017 DIVIDE nada
21026 ; 018 STORE PROM   PROM = SUMA / N 
11026 ; 019 WRITE PROM
43000 ; 020 HALT

; ----- Area de datos -----
00000   ; 021 N
00000   ; 022 X
00000   ; 023 SUMA
00000   ; 024 CONT
00001   ; 025 UNO  (constante 1)
00000   ; 026 PROM
99999   ; fin del programa