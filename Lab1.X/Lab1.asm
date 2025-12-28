include P18F4550.inc

aux1 equ 0h
aux2 equ 1h
aux3 equ 2h
aux4 equ 3h

Inicio
  
  ;1. Sumar 3 a la variable aux1 cargada previamente con el valor de 7.
  movlw .7
  movwf aux1
  addlw .3
    
  ;2. Sumar las variables aux1 y aux2 previamente cargadas con los valores de 
  ;   8 y 10 respectivamente.
  movlw .8
  movwf aux1
  movlw .10
  movwf aux2
  addwf aux1
  
  ;3. Restar a la constante 9 la variable aux1 cargada previamente con el valor
  ;   de 5.
  movlw .5
  movwf aux1
  sublw .9
  
  ;4. Restar la variable aux1 a la variable aux2 previamente cargadas con los 
  ;   valores de 6 y 4 respectivamente.
  movlw .6
  movwf aux1
  movlw .4
  movwf aux2
  movf aux1,w
  subwf aux2
  
  ;5. Multiplicar a la constante 4 la variable aux1 cargada previamente con el 
  ;   valor de 5.
  movlw .5
  movwf aux1
  mullw .4
  
  ;6. Multiplicar la variable aux1 a la variable aux2 previamente cargadas con 
  ;   los valores de 12 y 15 respectivamente.
  movlw .12
  movwf aux1
  movlw .15
  movwf aux2
  mulwf aux1
  
  ;7. Realizar el complemento a 1 de la variable aux1 cargada previamente con 
  ;   el valor de 12.
  movlw .12
  movwf aux1
  comf aux1
  
  ;8. Realizar el complemento a 2 de la variable aux1 cargada previamente con 
  ;   el valor de 12.
  movlw .12
  movwf aux1
  negf aux1
  
  ;9. Realizar la O lógica ?bitwise? entre el valor de 7 y la variable aux1 
  ;   cargada previamente con valor de 35.
  movlw .35
  movwf aux1
  iorlw .7
  
  ;10. Realizar la O lógica ?bitwise? entre las variables aux1 y aux2 cargadas 
  ;    previamente con los valores de 20 y 56 respectivamente.
  movlw .20
  movwf aux1
  movlw .56
  movwf aux2
  iorwf aux1
  
  ;11. Realizar la Y lógica ?bitwise? entre el valor de 15 y la variable aux1 
  ;    cargada previamente con el valor de 62.
  movlw .62
  movwf aux1
  andlw .15

  ;12. Realizar la Y lógica ?bitwise? entre las variables aux1 y aux2 cargadas 
  ;    previamente con los valores de 100 y 45 respectivamente.
  movlw .100
  movwf aux1
  movlw .45
  movwf aux2
  andwf aux1
  
  ;13. Realizar la O lógica exclusiva ?bitwise? entre el valor de 1 y la 
  ;    variable aux1 cargada previamente con el valor de 120.
  movlw .120
  movwf aux1
  xorlw .1
  
  ;14. Realizar la O lógica exclusiva ?bitwise? entre las variables aux1 y aux2 
  ;    cargadas previamente con los valores de 17 y 90 respectivamente.
  movlw .17
  movwf aux1
  movlw .90
  movwf aux2
  xorwf aux1
  
  ;15. Con aux1, aux2, y aux3, previamente cargadas con 25, 40, y 103 
  ;    respectivamente:
  ;    aux4 <- (aux1 OR aux2) AND (aux3 XOR 0xD0)
  movlw .25
  movwf aux1
  movlw .40
  movwf aux2
  movlw .103
  movwf aux3
  
  xorlw 0xD0
  movwf aux4
  
  movf aux1,w
  iorwf aux2
  
  movf aux2,w
  andwf aux4
  
  ;16. Con aux1, aux2, y aux3, previamente cargadas con 18, 60, y 16
  ;    respectivamente:
  ;    aux4 <- (aux1 + aux2) - 3 * (aux3 - 0b11010)
  movlw .18
  movwf aux1
  movlw .60
  movwf aux2
  movlw .16
  movwf aux3
  
  movlw b'11010'
  subwf aux3
  
  movf aux1,w
  addwf aux2
  
  movlw .3
  movwf aux4
  negf aux4
  movf aux3,w
  mulwf aux4
  
  movf PRODL,w
  addwf aux2

  goto Inicio
end