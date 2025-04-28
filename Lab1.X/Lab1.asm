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
 ;El valor más grande (decimal) que se puede cargar a la variable var1 es 255
 ;256 es leido como 0 (8 bits)
 movf aux2,w	;Llevar el valor de la variable var1 al sistema
 addlw .5	;Sumar la constante 5 al valor de la variable var1
 movwf aux3	;Llevar el resultado de la suma del sistema hasta la variable var2
 ;______________________________________________________________________________
 movlw .7
 movwf aux1
 movf aux1,w
 addlw .3
 movwf aux4
 ;________________________
 movlw .8
 movwf aux1
 movlw .10
 movwf aux2
 movf aux1,w
 addwf aux2	;Guarda la suma en variable aux2
 ;___________________________
 movlw .5
 movwf aux1
 movf aux1,w
 sublw .9	;Resta w de constante 9
 movwf aux2	;Guarda 9-5 en aux2
 ;__________________________________________
 movlw .6
 movwf aux1
 movlw .4
 movwf aux2
 movf aux1,w
 subwf aux2
end
