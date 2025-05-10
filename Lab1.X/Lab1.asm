include P18F4550.inc
 
 ;4 variables
aux1 equ 0h
aux2 equ 1h
aux3 equ 2h
aux4 equ 3h
 
Inicio
 ;Primer paso: Cargar 9 a var1
 movlw .5	;Crea constante
 movwf aux1	;Guarda la constante en variable var1
 movlw .9	
 movwf aux2
 ;El valor m�s grande (decimal) que se puede cargar a la variable var1 es 255
 ;256 es leido como 0 (8 bits)
 movf aux2,w	;Llevar el valor de la variable var1 al sistema
 addlw .5	;Sumar la constante 5 al valor de la variable var1
 movwf aux3	;Llevar el resultado de la suma a la variable var2
 ;__________________________________________________________Inicio de enunciados
 ;Sumar 3 a la variable aux1 cargada previamente con el valor de 7.
 movlw .7
 movwf aux1
 movf aux1,w
 addlw .3	
 movwf aux4
 ;______________________________________________________________________________
 ;Sumar las variables aux1 y aux2 previamente cargadas
 ;con los valores de 8 y 10 respectivamente.
 movlw .8
 movwf aux1
 movlw .10
 movwf aux2
 movf aux1,w
 addwf aux2	;Guarda la suma en variable aux2
 ;______________________________________________________________________________
 ;Restar a la constante 9 la variable aux1 cargada previamente
 ;con el valor de 5.
 movlw .5
 movwf aux1
 movf aux1,w
 sublw .9	;Resta w de constante 9
 movwf aux2	;Guarda 9-5 en aux2
 ;______________________________________________________________________________
 ;Restar la variable aux1 a la variable aux2 previamente cargadas
 ;con los valores de 6 y 4 respectivamente.
 movlw .6
 movwf aux1
 movlw .4
 movwf aux2
 movf aux1,w
 subwf aux2
 ;______________________________________________________________________________
 ;Multiplicar a la constante 4 la variable aux1 cargada previamente
 ;con el valor de 5.
 movlw .5
 movwf aux1
 movf aux1,w
 mullw .2
 ;______________________________________________________________________________
 ;Multiplicar la variable aux1 a la variable aux2 previamente cargadas
 ;con los valores de 12 y 15 respectivamente.
 movlw .12
 movwf aux1
 movlw .15
 movwf aux2
 mulwf aux1
 ;______________________________________________________________________________
 ;Realizar el complemento a 1 de la variable aux1 cargada previamente
 ;con el valor de 12.
 movlw .12
 movwf aux1
 comf aux1
 ;______________________________________________________________________________ 
 ;Realizar el complemento a 2 de la variable aux1 cargada previamente
 ;con el valor de 12.
 movlw .12
 movwf aux1
 comf aux1	;En aux1 queda complemento a 1
 movf aux1,w
 addlw .1	;Para complemento a 2 se suma 1
 movwf aux1
 ;______________________________________________________________________________
 ;Realizar la O l�gica ?bitwise? entre el valor de 7 y la variable aux1
 ;cargada previamente con el valor de 35.
 movlw .35
 movwf aux1
 iorlw .7
 movwf aux2
 ;______________________________________________________________________________
 ;Realizar la O l�gica ?bitwise? entre las variables aux1 y aux2
 ;cargadas previamente con los valores de 20 y 56 respectivamente.
 movlw .20
 movwf aux1
 movlw .56
 movwf aux2
 iorwf aux1
 movwf aux3	;Irrelevante, OR sobreescribe en aux1, no WREG
 ;______________________________________________________________________________
 ;Realizar la Y l�gica ?bitwise? entre el valor de 15 y la variable aux1
 ;cargada previamente con el valor de 62.
 movlw .62
 movwf aux1
 andlw .15
 movwf aux2
 ;______________________________________________________________________________
 ;Realizar la Y l�gica ?bitwise? entre las variables aux1 y aux2
 ;cargadas previamente con los valores de 100 y 45 respectivamente.
 movlw .100
 movwf aux1
 movlw .45
 movwf aux2
 andwf aux1
 movwf aux3	;Irrelevante, AND sobreescribe en aux1, no WREG
 ;______________________________________________________________________________
 ;Realizar la O l�gica exclusiva ?bitwise? entre el valor de 1
 ;y la variable aux1 cargada previamente con el valor de 120.
 movlw .120
 movwf aux1
 xorlw .1
 ;movwf aux3
 ;______________________________________________________________________________
 ;Realizar la O l�gica exclusiva ?bitwise? entre las variables aux1 y aux2
 ;cargadas previamente con los valores de 17 y 90 respectivamente.
 movlw .17
 movwf aux1
 movlw .90
 movwf aux2
 xorwf aux1
 ;movwf aux3	;Irrelevante, XOR sobreescribe en aux1, no WREG
 ;______________________________________________________________________________
 ;Seguido se debe realizar la siguiente operaci�n,
 ;con aux1, aux2 y aux3 previamente cargadas con 25, 40 y 103 respectivamente:
 ;aux4 <- (aux1 OR aux2) AND (aux3 XOR 0xD0)
 movlw .25
 movwf aux1
 movlw .40
 movwf aux2
 movlw .103
 movwf aux3

 movf aux1,w
 iorwf aux2,w
 movwf aux4

 movf aux3,w
 xorlw 0xD0

 andwf aux4,w
 movwf aux4

 ;Finalmente se debe realizar la siguiente operaci�n, con aux1, aux2 y aux3
 ;previamente cargadas con 18, 60 y 16 respectivamente:
 ;aux4 <- (aux1 + aux2) - 3 * (aux3 - 0b11010)
 movlw .18
 movwf aux1
 movlw .60
 movwf aux2
 movlw .16
 movwf aux3

 movf aux1,w
 addwf aux2,w
 movwf aux4

 ; Realizamos la operacion A-B = A + (~B+1)
 movlw .26
 movwf aux2
 comf aux2,w
 addlw .1
 addwf aux3,w
 movwf aux2

 movf aux2,w
 mullw .3
 movwf aux3

 movf aux4,w
 subwf aux3,w
 movwf aux4
 
 ;Fin clase
end
